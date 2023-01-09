#include "HistoryStorage.h"

void HistoryStorage::Add(const ImportBodyMessage::ImportBodyItem& item)
{
    if (item._systemItemType == SystemItemType::FOLDER)
        return;
    (*this)[item.id] = item;
    if (size() > _max_available_size)
        DeleteOld(item.date_ms);
}

void HistoryStorage::DeleteOld(long long date)
{
    vector<string> old;
    for (const auto& [id, item] : *this)
        if (item.date_ms < date - day_ms)
            old.push_back(id);
    for (const auto& it : old)
        erase(it);
    _max_available_size = size() * 2;
}

void HistoryStorage::Remove(const string &id)
{
    erase(id);
}

vector<ImportBodyMessage::ImportBodyItem> HistoryStorage::GetAll() const {
    auto ans = vector<ImportBodyMessage::ImportBodyItem>();
    for (const auto& [id, item]: *this)
        ans.push_back(item);
    return ans;
}

void HistoryStorage::Remove(const ImportBodyMessage::ImportBodyItem &item) {
    if (item._systemItemType == SystemItemType::FOLDER)
        return;
    Remove(item.id);
}
