#ifndef YAD_ECHOSERVER_HPP
#define YAD_ECHOSERVER_HPP
#include "QueryResponder.hpp"

class EchoServer:QueryResponder {
    Respond Response(const Query& query) override {
        return {0, ""};
    }
};
#endif //YAD_ECHOSERVER_HPP
