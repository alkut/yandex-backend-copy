#ifndef LIB_VALIDATOR_HPP
#define LIB_VALIDATOR_HPP

#include <boost/date_time/posix_time/posix_time.hpp>

#include "QueryExt.hpp"
#include "DeleteItem.hpp"
#include "../view/ImportBody.hpp"
#include "../logging/init.hpp"

namespace yad_server::validation {
const boost::posix_time::ptime origin = boost::posix_time::time_from_string("2000-01-01 00:00:00.000");

class Validator {
public:
    Validator() = default;
    enum class QueryTypes {IMPORT, DELETE, GET_NODES, UPDATE};
    QueryTypes GetType(const QueryExt& query) const ;
    void Validate(QueryExt& query, QueryTypes type) const ;
    static view::import_body_message::ImportBodyMessage GetImport(const QueryExt& query);
    static DeleteItem GetDelete(const QueryExt& query);
    static std::string GetNodes(const QueryExt& query);
    static std::string GetUpdate(const QueryExt& query);
    static long long check_datetime(const std::string& date);
private:
    const std::unordered_map<std::string, QueryTypes> types =
            {
                    {"imports", QueryTypes::IMPORT},
                    {"delete", QueryTypes::DELETE},
                    {"nodes", QueryTypes::GET_NODES},
                    {"updates", QueryTypes::UPDATE}
            };
    const std::unordered_map<QueryTypes, std::vector<std::function<void(QueryExt&)>>> constraints =
            {
                    {
                        QueryTypes::IMPORT, {
                            [](QueryExt &query) { check_count_sub_url(query, 2); }
                        }
                    },
                    {
                        QueryTypes::DELETE, {
                            [](QueryExt &query) { check_count_sub_url(query, 3); },
                            [](QueryExt &query) { check_datetime(query.params.at("date")); }

                        }
                    },
                    {
                        QueryTypes::GET_NODES, {
                            [](QueryExt &query) { check_count_sub_url(query, 3); }
                        }
                    },
                    {
                        QueryTypes::UPDATE, {
                            [](QueryExt &query) { check_count_sub_url(query, 2); },
                            [](QueryExt &query) { check_datetime(query.params.at("date")); }
                        }
                    }
            };
    static void check_count_sub_url(const QueryExt& query, size_t count);
};
    }
#endif //LIB_VALIDATOR_HPP
