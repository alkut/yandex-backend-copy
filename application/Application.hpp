#ifndef YAD_APPLICATION_HPP
#define YAD_APPLICATION_HPP

#include <event2/buffer.h>
#include "QueryResponder.hpp"
#include "Configuration.h"
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <event2/event_compat.h>

void ReadBody(struct evhttp_request* remote_rsp, std::vector<char>& v);

Query make_query(const std::string& Uri, std::vector<char>& body);

void OnRequest2(evhttp_request *req, void * _server);

template <class Responder>
class Application {
public:
    void run() {
        if (!event_init()) {
            // std::cerr << "Failed to init http server." << std::endl;
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }
        std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
        if (!Server)
        {
            // std::cerr << "Failed to init http server." << std::endl;
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }

        LOG(INFO) << "Server started";
        evhttp_set_gencb(Server.get(), OnRequest2, responder);
        if (event_dispatch() == -1)
        {
            // std::cerr << "Failed to run message loop." << std::endl;
        }
    }

private:
    Responder* responder = new Responder();
};

#endif //YAD_APPLICATION_HPP
