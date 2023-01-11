#ifndef LIB_VALIDATOR_HPP
#define LIB_VALIDATOR_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include "QueryExt.hpp"
#include "src/GlobalUtils.h"
#include "DeleteItem.hpp"

class Validator {
public:
    Validator() = default;
    enum class QueryTypes {IMPORT, DELETE, GET_NODES, UPDATE};
    QueryTypes GetType(const QueryExt& query) const ;
    void Validate(QueryExt& query, QueryTypes type) const ;
    static ImportBodyMessage GetImport(const QueryExt& query);
    static DeleteItem GetDelete(const QueryExt& query);
    static string GetNodes(const QueryExt& query);
    static string GetUpdate(const QueryExt& query);
private:
    std::unordered_map<std::string, QueryTypes> types =
            {
                    {"imports", QueryTypes::IMPORT},
                    {"delete", QueryTypes::DELETE},
                    {"nodes", QueryTypes::GET_NODES},
                    {"updates", QueryTypes::UPDATE}
            };
    std::unordered_map<QueryTypes, std::vector<std::function<void(QueryExt&)>>> constraints =
            {
                    {
                        QueryTypes::IMPORT, {
                            [](QueryExt &query) { check_count_sub_url(query, 1); }
                        }
                    },
                    {
                        QueryTypes::DELETE, {
                            [](QueryExt &query) { check_count_sub_url(query, 2); },
                            [](QueryExt &query) { check_datetime(query.params.at("date")); }

                        }
                    },
                    {
                        QueryTypes::GET_NODES, {
                            [](QueryExt &query) { check_count_sub_url(query, 2); }
                        }
                    },
                    {
                        QueryTypes::UPDATE, {
                            [](QueryExt &query) { check_count_sub_url(query, 1); },
                            [](QueryExt &query) { check_datetime(query.params.at("date")); }
                        }
                    }
            };

    static long long check_datetime(const string& date);
    static void check_count_sub_url(const QueryExt& query, size_t count);
};
#endif //LIB_VALIDATOR_HPP
