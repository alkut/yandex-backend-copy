#ifndef SERVER_QUERY
#define SERVER_QUERY

#include <string>
#include <unordered_map>

struct Query {
    std::string url;
    std::unordered_map<std::string, std::string> params;
    std::string body;
};

#endif // SERVER_QUERY