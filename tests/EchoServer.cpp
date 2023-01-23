#include "EchoServer.hpp"
#include "src/Includes.h"

Respond EchoServer::Response(const query::Query &query) {
    return {HTTP_OK, json{query}.dump(2)};
}