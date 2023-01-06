#include "Application.hpp"

void ReadBody(struct evhttp_request* remote_rsp, std::vector<char>& v) {
    v.clear();
    char buf[4096];
    struct evbuffer* buffer = evhttp_request_get_input_buffer(remote_rsp);
    int n = 0;
    while ((n = evbuffer_remove(buffer, buf, 4096)) > 0) {
        for (int i = 0; i < n; ++i)
            v.push_back(buf[i]);
    }
}

Query make_query(const std::string& Uri, std::vector<char>& body) {
    // position of question mark
    size_t qmark = Uri.find("?");

    // if there is no question mark then it is only a URL
    if (qmark == std::string::npos)
    {
        return {Uri, {}, std::string(body.begin(), body.end())};
    }

    // otherwise parsing parameters required
    // things to return
    std::unordered_map<std::string, std::string> params_um;
    std::string url = std::string(Uri.begin(), Uri.begin() + qmark);

    // intermediate variables -> split by "&"
    std::string params_string = std::string(Uri.begin() + qmark + 1, Uri.end());
    std::vector<std::string> params_vector;
    boost::split(params_vector, params_string, boost::is_any_of("&"));

    // split by "="
    for (const auto &params: params_vector) {
        std::vector<std::string> key_value;
        boost::split(key_value, params, boost::is_any_of("="));
        if (key_value.size() != 2)
        {
            LOG(ERROR) << "invalid parameters of query: " << params;
            throw std::invalid_argument( "invalid parameters of query.");

        }
        params_um[key_value[0]] = key_value[1];
    }
    return {url, params_um, std::string(body.begin(), body.end())};
}

void OnRequest2(evhttp_request *req, void * _server) {
    int ResponseCode = HTTP_OK;
    std::string ResponseMsg;
    auto *OutBuf = evhttp_request_get_output_buffer(req);
    if (!OutBuf)
        return;
    std::vector<char> body;
    ReadBody(req, body);
    std::string Uri(req->uri);
    if (body.empty())
        LOG(INFO) << "Got request  " << Uri;
    else
        LOG(INFO) << "Got request  " << Uri << " " << body.data();
    Query query;
    try
    {
        query = make_query(Uri, body);
    }
    catch (std::invalid_argument const& ex)
    {
        // return bad request
    }
    Respond respond = reinterpret_cast <QueryResponder*>(_server)->Response(query);
    evbuffer_add_printf(OutBuf, "%s", respond.message.c_str());
    evhttp_send_reply(req, respond.code, "", OutBuf);
}