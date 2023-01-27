#ifndef YAD_ECHOSERVER_HPP
#define YAD_ECHOSERVER_HPP
#include "../src/application/QueryResponder.hpp"
namespace yad_server::tests {
    class EchoServer : application::QueryResponder {
        yad_server::application::Respond Response(const yad_server::application::query::Query &query) override;
    };
}
#endif //YAD_ECHOSERVER_HPP
