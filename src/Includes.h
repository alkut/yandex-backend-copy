#ifndef SERVER_INCLUDES_H
#define SERVER_INCLUDES_H

#include <memory>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <evhttp.h>
#include <vector>
#include <string>
#include "cmake-build-debug/_deps/json-src/include/nlohmann/json.hpp"
#include <unordered_set>
#include <regex>
#include <cstdint> 
#include <unordered_map>
#include <initializer_list>
#include <utility>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <filesystem>

using json = nlohmann::json;
using string = std::string;
using regex = std::regex;

enum class SystemItemType {FILE, FOLDER};

template<class T>
using vector = std::vector<T>;
template<class T>
using unordered_set = std::unordered_set<T>;
template<class T, class V>
using unordered_map = std::unordered_map<T,V>;
template<class T, class V>
using map = std::map<T,V>;
typedef boost::adjacency_list<> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::vector< Vertex > container;

const json NullJson;
const json EmptyJson(json::value_t::array);

#endif //SERVER_INCLUDES_H
