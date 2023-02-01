#include "EchoServer.hpp"
namespace yad_server::tests {
    yad_server::application::Respond EchoServer::Response(const yad_server::application::query::Query &query) {
        return {HTTP_OK, nlohmann::json{query}.dump(2)};
    }
}