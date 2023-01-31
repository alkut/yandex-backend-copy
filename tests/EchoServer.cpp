#include "EchoServer.hpp"

Respond EchoServer::Response(const query_namespace::Query &query) {
    return {HTTP_OK, json{query}.dump(2)};
}