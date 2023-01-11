#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H

#include "src/Includes.h"
#include "src/View/ImportBody.h"
#include "src/Configuration.h"
#include "src/GlobalUtils.h"
#include "FileSystemTree.h"
#include "src/validation/Validator.hpp"
#include "src/application/QueryResponder.hpp"

class HttpServer : QueryResponder
{
public:
    HttpServer() = default;
    Respond Response(const Query& query) override;
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
        file_system.Import(msg);
    }
    void Delete(const string& id, const string& date, int& ResponseCode, string& ResponseMsg)
    {
        const auto& it = file_system.position.find(id);
        if (it == file_system.position.end())
        {
            ResponseCode = HTTP_NOTFOUND;
            LOG(ERROR) << "Item with id " << id << " not found";
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

        file_system.Delete(it->second, date, ms);
    }
    void GetNodes(const string& id, int& ResponseCode, string& ResponseMsg)
    {
        const auto& it = file_system.position.find(id);
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

        const auto& it = file_system.position.find(id);
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
    long long max_date = LLONG_MIN;
    Validator validator;

  void ExpandMessageValidateDate(ImportBodyMessage& msg, int& ResponseCode, string& ResponseMsg)
  {
      string date = msg.updateDate;
      long long ms = ValidateDate(msg.updateDate, ResponseCode, ResponseMsg);
      max_date = std::max(max_date, ms);
      if (ResponseCode != HTTP_OK)
          return;
      for (auto& it : msg.Items) {
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
          const auto& it = file_system.position.find(el.parentId);
          if (it == file_system.position.end()) {
              LOG(ERROR) << "No parent for id: " << el.id;
              ResponseCode = 400;
              return;
          }
          if (it->second->item._systemItemType == SystemItemType::FILE) {
                LOG(ERROR) << "Parent is file";
                ResponseCode = 400;
                return;
          }
      }
      const auto it = file_system.position.find(el.id);
      if (it != file_system.position.end() && el._systemItemType != it->second->item._systemItemType)
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
