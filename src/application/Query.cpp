#include "Query.hpp"

namespace yad_server::application::query {
    [[maybe_unused]] void to_json(nlohmann::json &j, const query::Query &query) {
        j = nlohmann::json{{"url",    query.url},
                           {"params", query.params},
                           {"body",   query.body}};
    }

    [[maybe_unused]] void from_json(const nlohmann::json &j, query::Query &query) {
        j.at("url").get_to(query.url);
        j.at("params").get_to(query.params);
        j.at("body").get_to(query.body);
    }
}