#include "ImportBody.hpp"
namespace yad_server::view {
    import_body_message::ImportBodyMessage::ImportBodyItem::ImportBodyItem(const nlohmann::json &j){
        from_json(j, *this);
    }
    void to_json(nlohmann::json& j, const import_body_message::ImportBodyMessage::ImportBodyItem& item){
        import_body_message::ImportBodyMessage::ImportBodyItem::AddAttribute(j, "id", item.id);
        import_body_message::ImportBodyMessage::ImportBodyItem::AddAttribute(j, "url", item.url);
        import_body_message::ImportBodyMessage::ImportBodyItem::AddAttribute(j, "parentId", item.parentId);
        import_body_message::ImportBodyMessage::ImportBodyItem::AddAttribute(j, "type", item.type);
        j["size"] = item.size;
        j["date"] = item.date;
    }
    void from_json(const nlohmann::json& j, import_body_message::ImportBodyMessage::ImportBodyItem& item){
        import_body_message::ImportBodyMessage::ImportBodyItem::GetAttribute(j, "id", item.id);
        import_body_message::ImportBodyMessage::ImportBodyItem::GetAttribute(j, "url", item.url);
        import_body_message::ImportBodyMessage::ImportBodyItem::GetAttribute(j, "parentId", item.parentId);
        import_body_message::ImportBodyMessage::ImportBodyItem::GetAttribute(j, "type", item.type);
        import_body_message::ImportBodyMessage::ImportBodyItem::GetAttribute(j, "size", item.size);
        if (item.type == "FILE") {
            item._systemItemType = SystemItemType::FILE;
        }
        if (item.type == "FOLDER") {
            item._systemItemType = SystemItemType::FOLDER;
        }
    }
    void import_body_message::ImportBodyMessage::ImportBodyItem::AddAttribute(nlohmann::json &j, const std::string &fieldName,
                                                         const std::string &value,
                                                         const std::string &defaultValue) {
        if (value != defaultValue)
            j[fieldName] = value;
        else
            j[fieldName] = NullJson;
    }

    template<typename T>
    void import_body_message::ImportBodyMessage::ImportBodyItem::GetAttribute(const nlohmann::json &j, const std::string &fieldName, T &field,
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
    void from_json(const nlohmann::json &j, import_body_message::ImportBodyMessage& item){
        item.updateDate = j["updateDate"];
        const auto &jsonItems = j["items"];
        size_t ItemsCount = jsonItems.size();
        item.Items.resize(ItemsCount);
        for (int i = 0; i < ItemsCount; ++i)
            from_json(jsonItems[i],item.Items[i]);
    }
}