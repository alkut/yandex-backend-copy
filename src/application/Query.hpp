// Copyright 2023 Alexsey Kutasov
#ifndef SRC_APPLICATION_QUERY_HPP_
#define SRC_APPLICATION_QUERY_HPP_

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace query {
struct Query {
    std::string url;
    std::unordered_map<std::string, std::string> params;
    std::string body;
};
[[maybe_unused]] void to_json(json &j, const Query &query);
[[maybe_unused]] void from_json(const json &j, Query &query);
};

#endif  // SRC_APPLICATION_QUERY_HPP_
