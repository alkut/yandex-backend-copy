#include "GlobalUtils.h"

string CodeMessage::Serialize() const
{
    json j;
    j["code"] = code;
    j["message"] = message;
    return j.dump(2);
}

string BadRequestResponse()
{
    CodeMessage cm{HTTP_BADREQUEST, val_message};
    return cm.Serialize();
}

string NotFoundResponse()
{
    CodeMessage cm{HTTP_NOTFOUND, item_message};
    return cm.Serialize();
}

string GetUpdateResponse(const vector<ImportBodyMessage::ImportBodyItem> &items)
{
    json j;
    json arr;
    for (const auto& it : items)
        arr.push_back(it.SerializeDump());
    j["items"] = arr;
    return j.dump(2);
}

string GetNodeHistoryResponse(const vector<ImportBodyMessage::ImportBodyItem> &items)
{
    return "Needs work";
}

void ValidateItem(const ImportBodyMessage::ImportBodyItem &item, int &ResponseCode, string &ResponseMsg)
{
    if (item.id.empty() || item.type.empty()) {
        ResponseCode = 400;
        LOG(ERROR) << "empty id or type";
        return;
    }
    if (item.type != "FILE" && item.type != "FOLDER") {
        ResponseCode = 400;
        LOG(ERROR) << "wrong type";
        return;
    }
    if (item._systemItemType == SystemItemType::FOLDER && item.size != 0) {
        ResponseCode = 400;
        LOG(ERROR) << "FOlder has non-zero size";
        return;
    }
    if (item._systemItemType == SystemItemType::FILE && item.size <= 0) {
        ResponseCode = 400;
        LOG(ERROR) << "File has non-positive size";
        return;
    }
    if (item._systemItemType == SystemItemType::FOLDER && !item.url.empty()) {
        ResponseCode = 400;
        LOG(ERROR) << "Folder has url";
        return;
    }
    if (item._systemItemType == SystemItemType::FILE && item.url.size() > max_url_size) {
        ResponseCode = 400;
        LOG(ERROR) << "File url is too long";
        return;
    }
}

long long ValidateDate(const string &date, int &ResponseCode, string &ResponseMsg)
{
    string s = date;
    std::replace(s.begin(), s.end(), 'T', ' ');
    s.resize(std::remove(s.begin(), s.end(), 'Z') - s.begin());
    try {
        auto delta = boost::posix_time::time_from_string(s) - origin;
        return static_cast<long long>(delta.total_milliseconds());
    }
    catch(const std::exception& ex)
    {
        LOG(ERROR) << "Date "<< date << "is not valid";
        ResponseCode = HTTP_BADREQUEST;
    }
    return 0;
}
