#ifndef SERVER_MIN_MAP_H
#define SERVER_MIN_MAP_H

#include "Includes.h"
#include "GlobalUtils.h"
#include "View/ImportBody.h"

template<typename time, typename Short, typename val>
struct MinMap {
  unordered_map<Short, val> u_m;
  map<time, unordered_set<Short>> m;
  unordered_map<Short, time> rm;

  void Add(time t, Short id, val item) {
    if (rm.find(id) != rm.end()) {
      m[rm[id]].erase(id);
    }
    u_m[id] = item;
    m[t].insert(id);
    rm[id] = t;
  }

  void Remove(Short id) {
    auto it = rm.find(id);
    if (it == rm.end()) {
      return;
    }
    m[it->second].erase(id);
    u_m.erase(it->first);
    rm.erase(it->first);
  }

  void RemoveMin(time t) {
    while (!m.empty() && m.begin()->first < t - day_in_ms) {
      while (!m.begin()->second.empty()) {
          Remove(*m.begin()->second.begin());
      }
      m.erase(m.begin()->first);
    }
  }
  std::vector<val> Update() {
    std::vector<val> result;
    for (const auto& [key, v] : u_m) {
      result.push_back(v);
    }
    return result;
  }
};

struct HistoryStorage : unordered_map<string, ImportBodyMessage::ImportBodyItem>
{
    void Add(const ImportBodyMessage::ImportBodyItem& item);
    void Remove(const string& id);
    void DeleteOld(long long date);
private:
    size_t _max_available_size = 100;
};

#endif // SERVER_MIN_MAP_H