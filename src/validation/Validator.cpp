#include "Validator.hpp"

Validator::QueryTypes Validator::GetType(const QueryExt &query) {
    return types.at(query.parsed_url[0]);
}

void Validator::Validate(QueryExt &query, const Validator::QueryTypes type) {
    for (const auto& constraint: constraints[type])
        constraint(query);
}


//TODO replays %3A -> ':' (why?)
long long Validator::check_datetime(const string &date) {
    string s = date;
    std::replace(s.begin(), s.end(), 'T', ' ');
    s.resize(std::remove(s.begin(), s.end(), 'Z') - s.begin());
    auto delta = boost::posix_time::time_from_string(s) - origin;
    return static_cast<long long>(delta.total_milliseconds());
}

void Validator::check_count_sub_url(const QueryExt &query, const size_t count) {
    if (query.parsed_url.size() != count) {
        throw std::invalid_argument("wrong count of sub url");
        LOG(ERROR) << "wrong count of sub url: " << query.parsed_url.size()
                   << "instead of " << count;
    }
}
