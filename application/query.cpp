//
// Created by krasikovir on 06/01/23.
//
#include <string>
#include <unordered_map>
#include <event2/http.h>
#include "Includes.h"
#include "Configuration.h"
#include "Model/HttpServer.h"
#include <system_error>
#include <boost/algorithm/string.hpp>

struct Query
{
    std::string url;
    std::unordered_map<std::string, std::string> params;
    std::string body;
};


struct Respond
{
    int code = HTTP_OK;
    std::string message;
};

class QueryResponder
{
public:
    virtual Respond Response(const Query& query) = 0;
};

class EchoServer:QueryResponder
{
    Respond Response(const Query& query) override
    {
        return {0, ""};
    }
};

void ReadChunkCallback(struct evhttp_request* remote_rsp, std::vector<char>& v)
{
    v.clear();
    char buf[4096];
    struct evbuffer* buffer = evhttp_request_get_input_buffer(remote_rsp);
    int n = 0;
    while ((n = evbuffer_remove(buffer, buf, 4096)) > 0)
    {
        for (int i = 0; i < n; ++i)
            v.push_back(buf[i]);
    }
}

Query make_query(const std::string& Uri, std::vector<char>& body)
{
    // position of question mark
    size_t qmark = Uri.find("?");

    // if there is no question mark then it is only a URL
    if (qmark == std::string::npos)
    {
        return {Uri, {}, std::string(body.begin(), body.end())};
    }

    // otherwise parsing parameters required
    else {
        // things to return
        std::unordered_map<string, string> params_um;
        std::string url = std::string(Uri.begin(), Uri.begin() + qmark);

        // intermediate variables -> split by "&"
        std::string params_string = std::string(Uri.begin() + qmark + 1, Uri.end());
        std::vector<std::string> params_vector;
        boost::split(params_vector, params_string, boost::is_any_of("&"));

        // split by "="
        for (const auto &params: params_vector) {
            std::vector<std::string> key_value;
            boost::split(key_value, params, boost::is_any_of("="));
            params_um[key_value[0]] = key_value[1];
        }
        return {url, params_um, std::string(body.begin(), body.end())};
    }
}

void OnRequest(evhttp_request *req, void * _server)
{
    int ResponseCode = HTTP_OK;
    string ResponseMsg;
    auto httpServer = (HttpServer *)_server;
    auto *OutBuf = evhttp_request_get_output_buffer(req);
    if (!OutBuf)
        return;
    std::vector<char> body;
    ReadChunkCallback(req, body);
    string Uri(req->uri);
    if (body.empty())
        LOG(INFO) << "Got request  " << Uri;
    else
        LOG(INFO) << "Got request  " << Uri << " " << body.data();
    //
    Query query = make_query(Uri, body);
    Respond respond = reinterpret_cast <QueryResponder*>(_server)->Response(query);
    evbuffer_add_printf(OutBuf, "%s", ResponseMsg.data());
    evhttp_send_reply(req, ResponseCode, "", OutBuf);
};

template <class Responder>
class Application
{
public:
    void run()
    {
        if (!event_init())
        {
            std::cerr << "Failed to init http server." << std::endl;
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }
        std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
        if (!Server)
        {
            std::cerr << "Failed to init http server." << std::endl;
            throw std::system_error(std::error_code(), "Failed to init http server.");
        }

        LOG(INFO) << "Server started";
        evhttp_set_gencb(Server.get(), OnRequest, responder);
        if (event_dispatch() == -1)
        {
            std::cerr << "Failed to run message loop." << std::endl;
        }
    }

private:
    Responder* responder = new Responder();
};
