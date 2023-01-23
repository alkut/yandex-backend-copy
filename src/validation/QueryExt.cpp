#include "QueryExt.hpp"

QueryExt::QueryExt(const Query &query) : Query(query) {
    boost::split(parsed_url, query.url, boost::is_any_of("/"));
}
