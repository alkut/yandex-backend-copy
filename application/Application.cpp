#include "Application.hpp"
<<<<<<< HEAD
#include <thread>

std::vector<char> ReadBody(struct evhttp_request* remote_rsp) {
    std::vector<char> v;
    char buf[4096];
    struct evbuffer* buffer = evhttp_request_get_input_buffer(remote_rsp);
    int n;
=======

std::vector<char> ReadBody(struct evhttp_request* remote_rsp) {
    std::vector<char> v;
    char buf[4096];
    struct evbuffer* buffer = evhttp_request_get_input_buffer(remote_rsp);
<<<<<<< HEAD
    int n = 0;
>>>>>>> 2971ad5 (YAS-12-add-application-template-class)
=======
    int n;
>>>>>>> fc06ffd (YAS-12-add-application-template-class)
    while ((n = evbuffer_remove(buffer, buf, 4096)) > 0) {
        for (int i = 0; i < n; ++i)
            v.push_back(buf[i]);
    }
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
=======
    return v;
>>>>>>> fc06ffd (YAS-12-add-application-template-class)
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
<<<<<<< HEAD
    boost::split(params_vector, params_string, boost::is_any_of("&"));

    // split by "="
>>>>>>> 2971ad5 (YAS-12-add-application-template-class)
=======
    boost::split(params_vector, splittedUri[1], boost::is_any_of("&"));
>>>>>>> fc06ffd (YAS-12-add-application-template-class)
    for (const auto &params: params_vector) {
        std::vector<std::string> key_value;
        boost::split(key_value, params, boost::is_any_of("="));
        if (key_value.size() != 2)
        {
<<<<<<< HEAD
<<<<<<< HEAD
            LOG(ERROR) << "invalid pair of parameters in query: " << params;
            throw std::invalid_argument( "invalid pair of parameters in query.");
=======
            LOG(ERROR) << "invalid parameters of query: " << params;
            throw std::invalid_argument( "invalid parameters of query.");
>>>>>>> 2971ad5 (YAS-12-add-application-template-class)
=======
            LOG(ERROR) << "invalid pair of parameters in query: " << params;
            throw std::invalid_argument( "invalid pair of parameters in query.");
>>>>>>> fc06ffd (YAS-12-add-application-template-class)

        }
        params_um[key_value[0]] = key_value[1];
    }
<<<<<<< HEAD
<<<<<<< HEAD
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

    if (Uri == "/shutdown") {
        PrintRespond(req, {HTTP_OK, "stop server\n"});
        reinterpret_cast <LibeventArgs*>(args)->stop_callback();
        return;
    }
    Query query;
    LOG(INFO) << "Got request  " << Uri << " " << (body.empty() ? "" : body.data());
    try {
        query = MakeQuery(Uri, body);
    }
    catch (std::exception const& ex) {
        PrintRespond(req, {HTTP_BADREQUEST, "Bad request\n"});
    }
    auto server = reinterpret_cast <LibeventArgs*>(args)->responder;
    Respond respond = reinterpret_cast <QueryResponder*>(server)->Response(query);
    PrintRespond(req, respond);
=======
    return {url, params_um, std::string(body.begin(), body.end())};
=======
    return {splittedUri[0], params_um, std::string(body.begin(), body.end())};
>>>>>>> fc06ffd (YAS-12-add-application-template-class)
}

void PrintRespond(struct evhttp_request * const req, const Respond& respond) {
    auto OutBuf = evhttp_request_get_output_buffer(req);
    if (!OutBuf)
        return;
    evbuffer_add_printf(OutBuf, "%s", respond.message.c_str());
    evhttp_send_reply(req, respond.code, "", OutBuf);
<<<<<<< HEAD
>>>>>>> 2971ad5 (YAS-12-add-application-template-class)
=======
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
>>>>>>> fc06ffd (YAS-12-add-application-template-class)
}