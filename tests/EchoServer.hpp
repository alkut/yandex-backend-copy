#ifndef YAD_ECHOSERVER_HPP
#define YAD_ECHOSERVER_HPP
#include "../src/application/QueryResponder.hpp"

class EchoServer:QueryResponder {
    Respond Response(const query_namespace::Query& query) override;
};
#endif //YAD_ECHOSERVER_HPP
