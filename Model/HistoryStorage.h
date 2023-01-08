#ifndef SERVER_MIN_MAP_H
#define SERVER_MIN_MAP_H

#include "Includes.h"
#include "GlobalUtils.h"
#include "View/ImportBody.h"

struct HistoryStorage : unordered_map<string, ImportBodyMessage::ImportBodyItem>
{
    void Add(const ImportBodyMessage::ImportBodyItem& item);
    void Remove(const string& id);
    void Remove(const ImportBodyMessage::ImportBodyItem& item);
    void DeleteOld(long long date);
    vector<ImportBodyMessage::ImportBodyItem> GetAll() const;
private:
    size_t _max_available_size = 100;
    const long long day_ms = 24 * 60 * 60 * 1000;
};

#endif // SERVER_MIN_MAP_H