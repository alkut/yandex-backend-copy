#ifndef SERVER_IMPORT_BODY_H
#define SERVER_IMPORT_BODY_H

#include <nlohmann/json.hpp>
namespace yad_server::view {
    enum class SystemItemType {
        FILE, FOLDER
    };

    const nlohmann::json NullJson;

    const nlohmann::json EmptyJson(nlohmann::json::value_t::array);

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

            void Deserialize(const nlohmann::json &j);

            void DeserializeExpanded(const nlohmann::json &j);

            [[nodiscard]] nlohmann::json SerializeDump() const;

            [[nodiscard]] std::string Serialize() const;

            [[nodiscard]] std::string SerializeExpanded() const;

        private:
            std::string _s_size;

            template<typename T>
            static void
            GetAttribute(const nlohmann::json &j, const std::string &fieldName, T &field, T defaultValue = T());

            static void AddAttribute(nlohmann::json &j, const std::string &fieldName, const std::string &value,
                                     const std::string &defaultValue = "");
        };

        std::vector<ImportBodyItem> Items;
        std::string updateDate;

        void Deserialize(const nlohmann::json &j);
    };
}
#endif //SERVER_IMPORT_BODY_H
