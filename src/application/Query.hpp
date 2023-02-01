#ifndef SERVER_QUERY
#define SERVER_QUERY

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
namespace yad_server::application::query {
            struct Query {
                std::string url;
                std::unordered_map<std::string, std::string> params;
                std::string body;
            };
            [[maybe_unused]] void to_json(nlohmann::json &j, const Query &query);

            [[maybe_unused]] void from_json(const nlohmann::json &j, Query &query);
} // end namespace yad_server::application::query
#endif // SERVER_QUERY