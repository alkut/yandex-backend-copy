#include "ImportBody.hpp"

ImportBodyMessage::ImportBodyItem::ImportBodyItem(const json &j) {
    DeserializeExpanded(j);
}

void ImportBodyMessage::ImportBodyItem::Deserialize(const json &j) {
    GetAttribute(j, "id", id);
    GetAttribute(j, "url", url);
    GetAttribute(j, "parentId", parentId);
    GetAttribute(j, "type", type);
    GetAttribute(j, "size",  size);
    if (type == "FILE") {
        _systemItemType = SystemItemType::FILE;
    }
    if (type == "FOLDER") {
        _systemItemType = SystemItemType::FOLDER;
    }
}

void ImportBodyMessage::ImportBodyItem::DeserializeExpanded(const json &j) {
    Deserialize(j);
    date_ms = j["date_ms"];
    date = j["date"];
    _systemItemType = j["_systemItemType"];
}

json ImportBodyMessage::ImportBodyItem::SerializeDump() const {
    json j;
    AddAttribute(j, "id", id);
    AddAttribute(j, "url", url);
    AddAttribute(j, "parentId", parentId);
    AddAttribute(j, "type", type);
    j["size"] = size;
    j["date"] = date;
    return j;
}

string ImportBodyMessage::ImportBodyItem::Serialize() const {
    return SerializeDump().dump(2);
}

string ImportBodyMessage::ImportBodyItem::SerializeExpanded() const {
    json j = SerializeDump();
    j["date_ms"] = date_ms;
    j["date"] = date;
    j["_systemItemType"] = _systemItemType;
    return j.dump(2);
}

void ImportBodyMessage::ImportBodyItem::AddAttribute(json &j, const string &fieldName, const string &value,
                                                     const string &defaultValue) {
    if (value != defaultValue)
        j[fieldName] = value;
    else
        j[fieldName] = NullJson;
}

template<typename T>
void ImportBodyMessage::ImportBodyItem::GetAttribute(const json &j, const string &fieldName, T &field, T defaultValue) {
    try {
        if (j.find(fieldName) != j.end() && !j[fieldName].is_null())
            field = j[fieldName];
        else
            field = defaultValue;
    }
    catch(const std::exception& ex) {
        string log_msg = "Try to get attribute ";
        string sss = "\nFailed with exception\n";
        string tmp = log_msg + fieldName + sss + ex.what();
    }
}

void ImportBodyMessage::Deserialize(const json &j) {
    updateDate = j["updateDate"];
    const auto& jsonItems = j["items"];
    size_t ItemsCount = jsonItems.size();
    Items.resize(ItemsCount);
    for (int i = 0; i < ItemsCount; ++i)
        Items[i].Deserialize(jsonItems[i]);
}
