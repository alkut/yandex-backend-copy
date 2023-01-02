#ifndef SERVER_MIN_MAP_H
#define SERVER_MIN_MAP_H

#include "Includes.h"
#include "GlobalUtils.h"
#include "View/ImportBody.h"

struct HistoryStorage : unordered_map<string, ImportBodyMessage::ImportBodyItem>
{
    void Add(const ImportBodyMessage::ImportBodyItem& item);
    void Remove(const string& id);
    void DeleteOld(long long date);
    vector<ImportBodyMessage::ImportBodyItem> GetAll() const;
private:
    size_t _max_available_size = 100;
};

#endif // SERVER_MIN_MAP_H