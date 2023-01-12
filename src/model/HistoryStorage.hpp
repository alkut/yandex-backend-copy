#ifndef SERVER_MIN_MAP_H
#define SERVER_MIN_MAP_H

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "src/view/ImportBody.h"

template<class T>
using vector = std::vector<T>;
template<class T>
using unordered_set = std::unordered_set<T>;
template<class T, class V>
using unordered_map = std::unordered_map<T,V>;

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