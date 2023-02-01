#ifndef YANDEX_BACKEND_COPY_QUERYSERIALIZATION_H
#define YANDEX_BACKEND_COPY_QUERYSERIALIZATION_H

#include "../../src/application/Query.hpp"
#include <nlohmann/json.hpp>

[[maybe_unused]] void to_json(nlohmann::json &j, const Query &query);

[[maybe_unused]] void from_json(const nlohmann::json &j, Query &query);

#endif //YANDEX_BACKEND_COPY_QUERYSERIALIZATION_H
