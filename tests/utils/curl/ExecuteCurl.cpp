#include "ExecuteCurl.hpp"
#include <iostream>
namespace yad_server::tests {
    std::string ExecuteCurl(std::string url,
                            std::unordered_map<std::string, std::string> params, std::string body) {
        CURL *curl_handle;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);
        curl_handle = curl_easy_init();
        std::string p = "?";
        for (const auto &[key, val]: params) p += key + "=" + val + '&';
        p.pop_back();
        std::string data;

        curl_easy_setopt(curl_handle, CURLOPT_URL, (url + p).c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
        return data;
    }

    static size_t
    WriteMemoryCallback(void *contents, size_t size, size_t nmemb, std::string &data) {
        std::string s(static_cast<char *>(contents), size * nmemb);
        data += s;
        return (size_t) data.size();
    }
}
