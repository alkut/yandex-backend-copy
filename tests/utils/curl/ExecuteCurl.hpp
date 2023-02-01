#ifndef LIB_EXECUTECURL_HPP
#define LIB_EXECUTECURL_HPP
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <curl/curl.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <stddef.h>

namespace yad_server::tests {
    std::string ExecuteCurl(std::string url, std::unordered_map<std::string,
            std::string> params, std::string body);

    static size_t
    WriteMemoryCallback(void *contents, size_t size, size_t nmemb, std::string &data);
}
#endif //LIB_EXECUTECURL_HPP
