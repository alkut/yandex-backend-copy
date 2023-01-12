#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H

#include "src/Includes.h"
#include "src/View/ImportBody.h"
#include "FileSystemTree.hpp"
#include "src/validation/Validator.hpp"
#include "src/application/QueryResponder.hpp"

class NotFoundException: std::exception {
public:
    explicit NotFoundException(const string& msg);
};

class HttpServer : QueryResponder
{
public:
    HttpServer() = default;
    Respond Response(const Query& query) override;
    FileSystemTree file_system;
private:
    long long max_date = LLONG_MIN;
    Validator validator;

//  void ExpandMessageValidateDate(ImportBodyMessage& msg, int& ResponseCode, string& ResponseMsg)
//  {
//      string date = msg.updateDate;
//      long long ms = ValidateDate(msg.updateDate, ResponseCode, ResponseMsg);
//      max_date = std::max(max_date, ms);
//      if (ResponseCode != HTTP_OK)
//          return;
//      for (auto& it : msg.Items) {
//          it.date = date;
//          it.date_ms = ms;
//      }
//  }

//  void _validateImport(const ImportBodyMessage& msg, int& ResponseCode, string& ResponseMsg) {
//    std::unordered_set<string> Ids;
//    std::unordered_map<string, ImportBodyMessage::ImportBodyItem> IdToItem;
//    for (auto& it : msg.Items)
//    {
//        IdToItem[it.id] = it;
//    }
//    for (const auto& el : msg.Items)
//    {
//      ValidateItem(el, ResponseCode, ResponseMsg);
//      if (ResponseCode != HTTP_OK)
//        return;
//    }
//
//    for (const auto& el : msg.Items)
//    {
//      Ids.insert(el.id);
//      if (!el.parentId.empty())
//      {
//          const auto& it = file_system.position.find(el.parentId);
//          if (it == file_system.position.end()) {
//              LOG(ERROR) << "No parent for id: " << el.id;
//              ResponseCode = 400;
//              return;
//          }
//          if (it->second->item._systemItemType == SystemItemType::FILE) {
//                LOG(ERROR) << "Parent is file";
//                ResponseCode = 400;
//                return;
//          }
//      }
//      const auto it = file_system.position.find(el.id);
//      if (it != file_system.position.end() && el._systemItemType != it->second->item._systemItemType)
//      {
//          LOG(ERROR) << "Change type of existed item";
//          ResponseCode = 400;
//          return;
//      }
//    }
//    if (Ids.size() < msg.Items.size())
//    {
//        LOG(ERROR) << "Id is not unique";
//        ResponseCode = 400;
//    }
//  }
    void ValidateImport(ImportBodyMessage &msg) const;
    FileSystemTree::Node *ValidateDelete(const string& id) const;
    FileSystemTree::Node *ValidateGetNodes(const string& id) const;
    static long long ValidateUpdate(const std::string& date);
    const Respond OK = {HTTP_OK, ""};
    const Respond NotFound = {HTTP_NOTFOUND, "item not found"};
    const Respond BadRequest = {HTTP_BADREQUEST, "validation failed"};
    void ValidateImportItem(ImportBodyMessage::ImportBodyItem &item, const std::unordered_set<std::string> &ids) const;
};

#endif //SERVER_HTTPSERVER_H
