// Copyright 2023 Alexsey Kutasov
#include "src/application/Query.hpp"
namespace query {
    [[maybe_unused]] void to_json(json &j, const query::Query &query) {
        j = json{{"url",    query.url},
                 {"params", query.params},
                 {"body",   query.body}};
    }

    [[maybe_unused]] void from_json(const json &j, query::Query &query) {
        j.at("url").get_to(query.url);
        j.at("params").get_to(query.params);
        j.at("body").get_to(query.body);
    }
}  // namespace query
