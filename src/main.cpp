#include "src/Configuration.h"
#include "src/OnRequest.h"

int main(int argc, char **argv)
{
    if (!event_init())
    {
        std::cerr << "Failed to init http server." << std::endl;
        return -1;
    }

    InitLogging(argv);
    std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
    if (!Server)
    {
        std::cerr << "Failed to init http server." << std::endl;
        return -1;
    }

    LOG(INFO) << "Server started";
    auto server = new HttpServer;
    evhttp_set_gencb(Server.get(), OnRequest, server);
    if (event_dispatch() == -1)
    {
        std::cerr << "Failed to run message loop." << std::endl;
        delete server;
        return -1;
    }
    delete server;
    return 0;
}
