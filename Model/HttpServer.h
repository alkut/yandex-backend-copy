#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H

#include "Includes.h"
#include "View/ImportBody.h"
#include "Configuration.h"
#include "GlobalUtils.h"
#include "Model/FileSystemTree.h"

class HttpServer
{
public:
    void Import(ImportBodyMessage& msg, int& ResponseCode, string& ResponseMsg) {
      ExpandMessageValidateDate(msg, ResponseCode, ResponseMsg);
      if (ResponseCode != HTTP_OK)
      {
          LOG(ERROR) << "Expand Message DateValidation failed";
          return;
      }

      _validateImport(msg, ResponseCode, ResponseMsg);
      if (ResponseCode != HTTP_OK)
      {
          LOG(ERROR) << "Validation Import failed";
          return;
      }
        LOG(INFO) << "Validation is good";
      file_system.Add(msg);
    }
    void Delete(const string& id, const string& date, int& ResponseCode, string& ResponseMsg)
    {
        const auto& it = file_system.position.find(file_system.IdToShort[id]);
        if (it == file_system.position.end())
        {
            ResponseCode = HTTP_NOTFOUND;
            LOG(ERROR) << "Item with id ", id, " not found";
            return;
        }

        long long ms = ValidateDate(date, ResponseCode, ResponseMsg);
        if (ResponseCode != HTTP_OK || ms < max_date)
        {
            if (ms < max_date)
                LOG(ERROR) << "Date " << date << "is less than before";
            ResponseCode = HTTP_BADREQUEST;
            return;
        }

        file_system.DeleteItem(it->second, date, ms);
    }
    void GetNodes(const string& id, int& ResponseCode, string& ResponseMsg)
    {
        const auto& it = file_system.position.find(file_system.IdToShort[id]);
        if (it == file_system.position.end())
        {
            ResponseCode = HTTP_NOTFOUND;
            LOG(ERROR) << "Item with id " << id << " is not found";
            return;
        }
        try {
            auto tmp = file_system.GetNodes(it->second);
            ResponseMsg = file_system.GetNodes(it->second).Serialize();
        }
        catch(const std::exception& ex)
        {
            LOG(ERROR) << "Oops! Something go wrong\n" << ex.what();
        }
    }
    void GetNodesHistory(const string& id, const string& DateStart, const string& DateEnd, int& ResponseCode, string& ResponseMsg)
    {
        long long start = ValidateDate(DateStart, ResponseCode, ResponseMsg);
        if (ResponseCode != HTTP_OK || start > max_date)
        {
            ResponseCode = HTTP_BADREQUEST;
            return;
        }

        long long end = ValidateDate(DateEnd, ResponseCode, ResponseMsg);
        if (ResponseCode != HTTP_OK || end > max_date || end <= start)
        {
            ResponseCode = HTTP_BADREQUEST;
            return;
        }

        const auto& it = file_system.position.find(file_system.IdToShort[id]);
        if (it == file_system.position.end())
        {
            LOG(ERROR) << "Item with id " << id << " not found";
            ResponseCode = HTTP_NOTFOUND;
            return;
        }
        try
        {
            ResponseMsg = GetNodeHistoryResponse(file_system.GetNodeHistory(it->second, start, end));
        }
        catch(const std::exception& ex)
        {
            LOG(ERROR) << "Oops! Something go wrong" << ex.what();
        }
    }
    void Update(const string& date, int& ResponseCode, string& ResponseMsg)
    {
        long long ms = ValidateDate(date, ResponseCode, ResponseMsg);
        if (ResponseCode != HTTP_OK || ms < max_date)
        {
            ResponseCode = HTTP_BADREQUEST;
            return;
        }
        try
        {
            ResponseMsg = GetUpdateResponse(file_system.Update(ms));
            LOG(INFO) << "Update return :" << ResponseMsg;
        }
        catch(const std::exception& ex)
        {
            LOG(ERROR) << "Oops! Something go wrong\n" << ex.what();
        }
    }
    FileSystemTree file_system;
private:

  long long max_date;

  void ExpandMessageValidateDate(ImportBodyMessage& msg, int& ResponseCode, string& ResponseMsg)
  {
      string date = msg.updateDate;
      long long ms = ValidateDate(msg.updateDate, ResponseCode, ResponseMsg);
      max_date = std::max(max_date, ms);
      if (ResponseCode != HTTP_OK)
          return;
      for (auto& it : msg.Items)
      {
          const auto& f = file_system.IdToShort.find(it.id);
          it.ShortId = (f != file_system.IdToShort.end()) ? f->second : 0;
          const auto& f2 = file_system.IdToShort.find(it.parentId);
          it.ShortParentId = (f2 != file_system.IdToShort.end()) ? f2->second : 0;
          it.date = date;
          it.date_ms = ms;
      }
  }

  void _validateImport(const ImportBodyMessage& msg, int& ResponseCode, string& ResponseMsg) {
    std::unordered_set<string> Ids;
    std::unordered_map<string, ImportBodyMessage::ImportBodyItem> IdToItem;
    for (auto& it : msg.Items)
    {
        IdToItem[it.id] = it;
    }
    for (const auto& el : msg.Items)
    {
      ValidateItem(el, ResponseCode, ResponseMsg);
      if (ResponseCode != HTTP_OK)
        return;
    }

    for (const auto& el : msg.Items)
    {
      Ids.insert(el.id);
      if (!el.parentId.empty())
      {
        if (el.ShortParentId > 0)
        {
          if (file_system.ShortToBody[el.ShortParentId]._systemItemType == SystemItemType::FILE)
          {
                LOG(ERROR) << "Parent is file";
                ResponseCode = 400;
                return;
          }
        }
        else
        {
            auto it = IdToItem.find(el.parentId);
            if (it == IdToItem.end())
            {
                LOG(ERROR) << "No parent for id: " << el.id;
                ResponseCode = 400;
                return;
            }
        }
      }
      if (el.ShortId > 0 && el._systemItemType != file_system.ShortToBody[el.ShortId]._systemItemType)
      {
          LOG(ERROR) << "Change type of existed item";
          ResponseCode = 400;
          return;
      }
    }
    if (Ids.size() < msg.Items.size())
    {
        LOG(ERROR) << "Id is not unique";
        ResponseCode = 400;
    }
  }
  int _validateDelete()
  {
    return HTTP_OK;
  }

  int _validateGetNodesHistory()
  {
    return HTTP_OK;
  }
  int _validateUpdate()
  {
    return HTTP_OK;
  }
};

#endif //SERVER_HTTPSERVER_H
