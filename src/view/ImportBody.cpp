#include "ImportBody.hpp"
namespace yad_server::view {
    ImportBodyMessage::ImportBodyItem::ImportBodyItem(const nlohmann::json &j) {
        DeserializeExpanded(j);
    }

    void ImportBodyMessage::ImportBodyItem::Deserialize(const nlohmann::json &j) {
        GetAttribute(j, "id", id);
        GetAttribute(j, "url", url);
        GetAttribute(j, "parentId", parentId);
        GetAttribute(j, "type", type);
        GetAttribute(j, "size", size);
        if (type == "FILE") {
            _systemItemType = SystemItemType::FILE;
        }
        if (type == "FOLDER") {
            _systemItemType = SystemItemType::FOLDER;
        }
    }

    void ImportBodyMessage::ImportBodyItem::DeserializeExpanded(const nlohmann::json &j) {
        Deserialize(j);
        date_ms = j["date_ms"];
        date = j["date"];
        _systemItemType = j["_systemItemType"];
    }

    nlohmann::json ImportBodyMessage::ImportBodyItem::SerializeDump() const {
        nlohmann::json j;
        AddAttribute(j, "id", id);
        AddAttribute(j, "url", url);
        AddAttribute(j, "parentId", parentId);
        AddAttribute(j, "type", type);
        j["size"] = size;
        j["date"] = date;
        return j;
    }

    std::string ImportBodyMessage::ImportBodyItem::Serialize() const {
        return SerializeDump().dump(2);
    }

    std::string ImportBodyMessage::ImportBodyItem::SerializeExpanded() const {
        nlohmann::json j = SerializeDump();
        j["date_ms"] = date_ms;
        j["date"] = date;
        j["_systemItemType"] = _systemItemType;
        return j.dump(2);
    }

    void ImportBodyMessage::ImportBodyItem::AddAttribute(nlohmann::json &j, const std::string &fieldName,
                                                         const std::string &value,
                                                         const std::string &defaultValue) {
        if (value != defaultValue)
            j[fieldName] = value;
        else
            j[fieldName] = NullJson;
    }

    template<typename T>
    void
    ImportBodyMessage::ImportBodyItem::GetAttribute(const nlohmann::json &j, const std::string &fieldName, T &field,
                                                    T defaultValue) {
        try {
            if (j.find(fieldName) != j.end() && !j[fieldName].is_null())
                field = j[fieldName];
            else
                field = defaultValue;
        }
        catch (const std::exception &ex) {
            std::string log_msg = "Try to get attribute ";
            std::string sss = "\nFailed with exception\n";
            std::string tmp = log_msg + fieldName + sss + ex.what();
        }
    }

    void ImportBodyMessage::Deserialize(const nlohmann::json &j) {
        updateDate = j["updateDate"];
        const auto &jsonItems = j["items"];
        size_t ItemsCount = jsonItems.size();
        Items.resize(ItemsCount);
        for (int i = 0; i < ItemsCount; ++i)
            Items[i].Deserialize(jsonItems[i]);
    }
}