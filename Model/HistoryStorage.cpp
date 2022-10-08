#include "Model/HistoryStorage.h"

void HistoryStorage::Add(const ImportBodyMessage::ImportBodyItem& item)
{
    (*this)[item.id] = item;
    if (size() > _max_available_size)
        DeleteOld(item.date_ms);
}

void HistoryStorage::DeleteOld(long long date)
{
    vector<string> old;
    for (const auto& [id, item] : *this)
        if (item.date_ms < date)
            old.push_back(id);
    for (const auto& it : old)
        erase(it);
    _max_available_size = size() * 2;
}

void HistoryStorage::Remove(const string &id)
{
    erase(id);
}
