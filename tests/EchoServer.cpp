#include "EchoServer.hpp"

Respond EchoServer::Response(const Query &query) {
    std::string params_string = "?";
    for (const auto &[key, value]: query.params) params_string += key + '=' + value + '&';
    params_string.pop_back(); params_string += '#';
    // params_string = http://localhost + ?key=value&key=value# + body

    return {HTTP_OK, query.url + params_string + query.body};
}