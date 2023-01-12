#ifndef LIB_QUERYEXT_HPP
#define LIB_QUERYEXT_HPP

#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include "src/application/Query.hpp"

struct QueryExt : Query {
    std::vector<std::string> parsed_url;
    explicit QueryExt(const Query& query);
};

#endif //LIB_QUERYEXT_HPP
