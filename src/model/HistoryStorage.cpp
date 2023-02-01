#include "HistoryStorage.hpp"
namespace yad_server::model {
        void HistoryStorage::Add(const view::import_body_message::ImportBodyMessage::ImportBodyItem &item) {
            if (item._systemItemType == view::SystemItemType::FOLDER)
                return;
            (*this)[item.id] = item;
            if (size() > _max_available_size)
                DeleteOld(item.date_ms);
        }

        void HistoryStorage::DeleteOld(long long date) {
            std::vector<std::string> old;
            for (const auto&[id, item]: *this)
                if (item.date_ms < date - day_ms)
                    old.push_back(id);
            for (const auto &it: old)
                erase(it);
            _max_available_size = size() * 2;
        }

        void HistoryStorage::Remove(const std::string &id) {
            erase(id);
        }

        std::vector<view::import_body_message::ImportBodyMessage::ImportBodyItem> HistoryStorage::GetAll() const {
            auto ans = std::vector<view::import_body_message::ImportBodyMessage::ImportBodyItem>();
            for (const auto&[id, item]: *this)
                ans.push_back(item);
            return ans;
        }

        void HistoryStorage::Remove(const view::import_body_message::ImportBodyMessage::ImportBodyItem &item) {
            if (item._systemItemType == view::SystemItemType::FOLDER)
                return;
            Remove(item.id);
        }
    }