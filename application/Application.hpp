#ifndef YAD_APPLICATION_HPP
#define YAD_APPLICATION_HPP

#include <event2/buffer.h>
#include "QueryResponder.hpp"
#include "Configuration.h"
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <event2/event_compat.h>
#include "LibeventArgs.hpp"

void ReadBody(struct evhttp_request* remote_rsp, std::vector<char>& v);

Query MakeQuery(const std::string& Uri, std::vector<char>& body);

void OnRequest2(evhttp_request *req, void * _server);

void PrintRespond(struct evhttp_request* req, const Respond& respond);

template <class Responder>
class Application {
public:
    evhttp *http_server = nullptr;
    void run() {
        if (!event_init()) {
            // std::cerr << "Failed to init http server." << std::endl;
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }
        http_server = evhttp_start(SrvAddress, SrvPort);
        if (http_server == nullptr) {
            // std::cerr << "Failed to init http server." << std::endl;
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }
        LOG(INFO) << "Server started";
        evhttp* http_server_local = http_server;
        auto stop_callback = [http_server_local]() {
            evhttp_free(http_server_local);
        };
        LibeventArgs libevent_args = {responder, stop_callback};
        evhttp_set_gencb(http_server, OnRequest2, &libevent_args);
        if (event_dispatch() == -1) {
            // std::cerr << "Failed to run message loop." << std::endl;
        }

    }

    ~Application() {
    };

private:
    Responder* responder = new Responder();
};

#endif //YAD_APPLICATION_HPP
