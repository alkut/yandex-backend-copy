#include "Application.hpp"

std::vector<char> ReadBody(struct evhttp_request* remote_rsp) {
    std::vector<char> v;
    char buf[4096];
    struct evbuffer* buffer = evhttp_request_get_input_buffer(remote_rsp);
    int n;
    while ((n = evbuffer_remove(buffer, buf, 4096)) > 0) {
        for (int i = 0; i < n; ++i)
            v.push_back(buf[i]);
    }
    return v;
}

Query MakeQuery(const std::string& Uri, const std::vector<char>& body) {
    std::vector<std::string> splittedUri;
    boost::split(splittedUri, Uri, boost::is_any_of("?"));
    if (splittedUri.size() == 1) {
        return {Uri, {}, std::string(body.begin(), body.end())};
    }
    if (splittedUri.size() > 2) {
        LOG(ERROR) << "invalid uri: ";
        throw std::invalid_argument( "invalid uri.");
    }
    std::unordered_map<std::string, std::string> params_um;
    std::vector<std::string> params_vector;
    boost::split(params_vector, splittedUri[1], boost::is_any_of("&"));
    for (const auto &params: params_vector) {
        std::vector<std::string> key_value;
        boost::split(key_value, params, boost::is_any_of("="));
        if (key_value.size() != 2)
        {
            LOG(ERROR) << "invalid pair of parameters in query: " << params;
            throw std::invalid_argument( "invalid pair of parameters in query.");

        }
        params_um[key_value[0]] = key_value[1];
    }
    return {splittedUri[0], params_um, std::string(body.begin(), body.end())};
}

void PrintRespond(struct evhttp_request * const req, const Respond& respond) {
    auto OutBuf = evhttp_request_get_output_buffer(req);
    if (!OutBuf)
        return;
    evbuffer_add_printf(OutBuf, "%s", respond.message.c_str());
    evhttp_send_reply(req, respond.code, "", OutBuf);
}

void OnRequest2(evhttp_request * const req, void * args) {
    std::vector<char> body = ReadBody(req);
    std::string Uri(req->uri);

    // define SHUTDOWN
    if (Uri == "shutdown") {
        PrintRespond(req, {HTTP_OK, "stop server\n"});
        reinterpret_cast <LibeventArgs*>(args)->stop_callback();
        return;
    }
    Query query;
    LOG(INFO) << "Got request  " << Uri << " " << body.data();
    try {
        query = MakeQuery(Uri, body);
    }
    catch (std::exception const& ex) {
        PrintRespond(req, {HTTP_BADREQUEST, "Bad request\n"});
    }
    auto server = reinterpret_cast <LibeventArgs*>(args)->responder;
    Respond respond = reinterpret_cast <QueryResponder*>(server)->Response(query);
    PrintRespond(req, respond);
}