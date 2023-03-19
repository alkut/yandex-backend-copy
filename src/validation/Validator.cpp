#include "Validator.hpp"
namespace yad_server::validation {
//TODO replays %3A -> ':' (why?) ( "replays XDDDD")
    std::string Validator::check_datetime(const std::string &date) {
        std::string s = date;
        std::replace(s.begin(), s.end(), 'T', ' ');
        s.resize(std::remove(s.begin(), s.end(), 'Z') - s.begin());
        boost::algorithm::replace_all(s, "%3A", ":");
        return s;
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
        return {query.parsed_url[2], check_datetime(query.params.at("date"))};
    }

    std::string Validator::GetNodes(const QueryExt &query) {
        return query.parsed_url[2];
    }
} // end of namespace yad_server::validation