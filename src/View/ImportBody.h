#ifndef SERVER_IMPORTBODY_H
#define SERVER_IMPORTBODY_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using string = std::string;

enum class SystemItemType {FILE, FOLDER};

const json NullJson;
const json EmptyJson(json::value_t::array);

struct ImportBodyMessage
{
    ImportBodyMessage() = default;

    struct ImportBodyItem
    {
        ImportBodyItem() = default;
        explicit ImportBodyItem(const json& j)
        {
            DeserializeExpanded(j);
        }
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
        void Deserialize(const json& j)
        {
            GetAttribute(j, "id", id);
            GetAttribute(j, "url", url);
            GetAttribute(j, "parentId", parentId);
            GetAttribute(j, "type", type);
            GetAttribute(j, "size",  size);
            if (type == "FILE")
            {
                _systemItemType = SystemItemType::FILE;
            }
            if (type == "FOLDER")
            {
                _systemItemType = SystemItemType::FOLDER;
            }
        }
        void DeserializeExpanded(const json& j)
        {
            Deserialize(j);
            date_ms = j["date_ms"];
            date = j["date"];
            _systemItemType = j["_systemItemType"];
        }
        [[nodiscard]] json SerializeDump() const
        {
            json j;
            AddAttribute(j, "id", id);
            AddAttribute(j, "url", url);
            AddAttribute(j, "parentId", parentId);
            AddAttribute(j, "type", type);
            j["size"] = size;
            j["date"] = date;
            return j;
        }
        [[nodiscard]] string Serialize() const
        {
            return SerializeDump().dump(2);
        }

        [[nodiscard]] string SerializeExpanded() const
        {
            json j = SerializeDump();
            j["date_ms"] = date_ms;
            j["date"] = date;
            j["_systemItemType"] = _systemItemType;
            return j.dump(2);
        }
    private:
        string _s_size;
		template<typename T>
        static void GetAttribute(const json& j, const string& fieldName, T& field,  T defaultValue = T())
        {
            try
            {
                if (j.find(fieldName) != j.end() && !j[fieldName].is_null())
                    field = j[fieldName];
                else
                    field = defaultValue;
            }
            catch(const std::exception& ex)
            {
                string log_msg = "Try to get attribute ";
                string sss = "\nFailed with exception\n";
                string tmp = log_msg + fieldName + sss + ex.what();
            }

        }

        static void AddAttribute(json& j, const string& fieldName, const string& value, const string& defaultValue = "")
        {
            if (value != defaultValue)
                j[fieldName] = value;
            else
                j[fieldName] = NullJson;
        }
    };
    std::vector<ImportBodyItem> Items;
    string updateDate;

    void Deserialize(const json& j)
    {
        updateDate = j["updateDate"];
        const auto& jsonItems = j["items"];
        size_t ItemsCount = jsonItems.size();
        Items.resize(ItemsCount);
        for (int i = 0; i < ItemsCount; ++i)
            Items[i].Deserialize(jsonItems[i]);
    }
};

#endif //SERVER_IMPORTBODY_H
