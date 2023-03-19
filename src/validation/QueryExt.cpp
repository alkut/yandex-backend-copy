#include "QueryExt.hpp"
namespace yad_server::validation {
    QueryExt::QueryExt(const Query &query) : Query(query) {
        boost::split(parsed_url, query.url, boost::is_any_of("/"));
    }
}