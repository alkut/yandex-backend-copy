#ifndef SERVER_IMPORT_BODY_H
#define SERVER_IMPORT_BODY_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using string = std::string;

enum class SystemItemType {FILE, FOLDER};

const json NullJson;
const json EmptyJson(json::value_t::array);

struct ImportBodyMessage {
    ImportBodyMessage() = default;
    struct ImportBodyItem {
        ImportBodyItem() = default;
        explicit ImportBodyItem(const json& j);
        //Serializable fields
        string id;
        string url;
        string parentId;
        string type;
        int64_t size = 0;
        //Expanded fields
        long long date_ms = 0ll;
        string date;
        SystemItemType _systemItemType = SystemItemType::FILE;
        void Deserialize(const json& j);
        void DeserializeExpanded(const json& j);
        [[nodiscard]] json SerializeDump() const;
        [[nodiscard]] string Serialize() const;
        [[nodiscard]] string SerializeExpanded() const;
    private:
        string _s_size;
		template<typename T>
        static void GetAttribute(const json& j, const string& fieldName, T& field,  T defaultValue = T());
        static void AddAttribute(json& j, const string& fieldName, const string& value, const string& defaultValue = "");
    };
    std::vector<ImportBodyItem> Items;
    string updateDate;
    void Deserialize(const json& j);
};

#endif //SERVER_IMPORT_BODY_H
