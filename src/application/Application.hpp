#ifndef YAD_APPLICATION_HPP
#define YAD_APPLICATION_HPP

#include <event2/buffer.h>
#include "QueryResponder.hpp"
#include "src/Configuration.h"
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <event2/event_compat.h>
#include "LibeventArgs.hpp"

std::vector<char> ReadBody(struct evhttp_request* remote_rsp);

Query MakeQuery(const std::string& Uri, const std::vector<char>& body);

void OnRequest2(evhttp_request * const req, void * _server);

void PrintRespond(struct evhttp_request* req, const Respond& respond);

///@params Responder: Responder class inherits from QueryResponder class.
///Responder class defines the query logic: Responder = {EchoServer, ApplicationServer}
///Application class abstracts libenevt usage from rest of the code
template <class Responder>
class Application {
public:
    evhttp *http_server = nullptr;
    void run() {
        if (!event_init()) {
            LOG(ERROR) << "Failed to init http server.";
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }
        http_server = evhttp_start(SrvAddress, SrvPort);
        if (http_server == nullptr) {
            LOG(ERROR) << "Failed to init http server.";
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }
        LOG(INFO) << "Server started";
        evhttp* http_server_local = http_server;
        auto stop_callback = [http_server_local]() {
            evhttp_free(http_server_local);
        };
        LibeventArgs libevent_args = {reinterpret_cast<QueryResponder*>(responder), stop_callback};
        evhttp_set_gencb(http_server, OnRequest2, &libevent_args);
        if (event_dispatch() == -1) {
            // std::cerr << "Failed to run message loop." << std::endl;
        }
    }

private:
    Responder* responder = new Responder();
};

#endif //YAD_APPLICATION_HPP