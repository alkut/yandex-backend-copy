#ifndef SERVER_MIN_MAP_H
#define SERVER_MIN_MAP_H

#include <unordered_map>
#include <vector>
#include <string>

#include "../view/ImportBody.hpp"

namespace yad_server::model {
struct HistoryStorage : std::unordered_map<std::string, view::import_body_message::ImportBodyMessage::ImportBodyItem> {
        void Add(const view::import_body_message::ImportBodyMessage::ImportBodyItem &item);

        void Remove(const std::string &id);

        void Remove(const view::import_body_message::ImportBodyMessage::ImportBodyItem &item);

        void DeleteOld(long long date);

        std::vector<view::import_body_message::ImportBodyMessage::ImportBodyItem> GetAll() const;

    private:
        size_t _max_available_size = 100;
        const long long day_ms = 24 * 60 * 60 * 1000;
    };
}

#endif // SERVER_MIN_MAP_H