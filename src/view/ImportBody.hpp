#ifndef SERVER_IMPORT_BODY_H
#define SERVER_IMPORT_BODY_H

#include <nlohmann/json.hpp>
namespace yad_server::view {
    enum class SystemItemType {
        FILE, FOLDER
    };

    const nlohmann::json NullJson;

    const nlohmann::json EmptyJson(nlohmann::json::value_t::array);
    namespace import_body_message {
        struct ImportBodyMessage {
            ImportBodyMessage() = default;

            struct ImportBodyItem {
                ImportBodyItem() = default;

                explicit ImportBodyItem(const nlohmann::json &j);

                //Serializable fields
                std::string id;
                std::string url;
                std::string parentId;
                std::string type;
                int64_t size = 0;
                //Expanded fields
                long long date_ms = 0ll;
                std::string date;
                SystemItemType _systemItemType = SystemItemType::FILE;

                template<typename T>
                static void
                GetAttribute(const nlohmann::json &j, const std::string &fieldName, T &field, T defaultValue = T());

                static void AddAttribute(nlohmann::json &j, const std::string &fieldName, const std::string &value,
                                         const std::string &defaultValue = "");

            private:
                std::string _s_size;
            };


            std::vector<ImportBodyItem> Items;
            std::string updateDate;

        };
    }
    [[maybe_unused]] void from_json(const nlohmann::json &j, import_body_message::ImportBodyMessage &item);

    [[maybe_unused]] void to_json(nlohmann::json &j, const import_body_message::ImportBodyMessage::ImportBodyItem &item);

    [[maybe_unused]] void from_json(const nlohmann::json &j, import_body_message::ImportBodyMessage::ImportBodyItem &item);
}
#endif //SERVER_IMPORT_BODY_H
