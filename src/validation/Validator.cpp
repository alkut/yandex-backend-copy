#include "Validator.hpp"
namespace yad_server::validation {
    Validator::QueryTypes Validator::GetType(const QueryExt &query) const {
        return types.at(query.parsed_url[1]);
    }

    void Validator::Validate(QueryExt &query, const Validator::QueryTypes type) const {
        for (const auto &constraint: constraints.at(type))
            constraint(query);
    }


//TODO replays %3A -> ':' (why?) ( "replays XDDDD")
    long long Validator::check_datetime(const std::string &date) {
        std::string s = date;
        std::replace(s.begin(), s.end(), 'T', ' ');
        s.resize(std::remove(s.begin(), s.end(), 'Z') - s.begin());
        boost::algorithm::replace_all(s, "%3A", ":");
        auto delta = boost::posix_time::time_from_string(s) - origin;
        return static_cast<long long>(delta.total_milliseconds());
    }

    void Validator::check_count_sub_url(const QueryExt &query, const size_t count) {
        if (query.parsed_url.size() != count) {
            LOG(ERROR) << "wrong count of sub url: " << query.parsed_url.size()
                       << "instead of " << count;
            throw std::invalid_argument("wrong count of sub url");
        }
    }

    view::import_body_message::ImportBodyMessage Validator::GetImport(const QueryExt &query) {
        auto body = view::import_body_message::ImportBodyMessage();
        view::from_json(nlohmann::json::parse(query.body),body);
        for (const auto &item: body.Items) {
            if (item._systemItemType == view::SystemItemType::FILE && item.size <= 0)
                throw std::invalid_argument("empty file");
            if (item._systemItemType == view::SystemItemType::FOLDER && item.size != 0)
                throw std::invalid_argument("not empty folder");
            if (item.url.size() > 255)
                throw std::invalid_argument("too big url");
        }
        return body;
    }

    DeleteItem Validator::GetDelete(const QueryExt &query) {
        return {query.parsed_url[2], query.params.at("date")};
    }

    std::string Validator::GetNodes(const QueryExt &query) {
        return query.parsed_url[2];
    }

    std::string Validator::GetUpdate(const QueryExt &query) {
        return query.params.at("date");
    }
}