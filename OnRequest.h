#ifndef SERVER_OnRequestUEST_H
#define SERVER_OnRequestUEST_H

#include "Includes.h"
#include "Model/HttpServer.h"
#include "UrlResolvers.h"
#include "View/ImportBody.h"
#include "GlobalUtils.h"

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
    if (std::regex_match(Uri, regexes::Import))
    {
        bool exception = false;
        ImportBodyMessage msg;
        try
        {
            msg.Deserialize(nlohmann::json::parse(body));
            LOG(INFO) << "Json parsed";
        }
        catch (const std::exception& ex)
        {
            LOG(ERROR) << "Json not parsed\nBody is \n" << body.data() <<
                    "\nFailed with exception\n" << ex.what();
            ResponseCode = HTTP_BADREQUEST;
            exception = true;
        }
        if (!exception)
            httpServer->Import(msg, ResponseCode, ResponseMsg);
    }
    else if (std::regex_match(Uri, regexes::Delete))
    {
        string id, date;
        regexes::ResolveDelete(Uri, id, date);
        httpServer->Delete(id, date, ResponseCode, ResponseMsg);
    }
    else if (std::regex_match(Uri, regexes::GetNodes))
    {
        string id;
        regexes::ResolveGetNodes(Uri, id);
        httpServer->GetNodes(id, ResponseCode, ResponseMsg);
        LOG(INFO) << "Nodes request completed with result\n" << ResponseMsg;
    }
    else if (std::regex_match(Uri, regexes::GetNodesHistory))
    {
        string id, startDate, endDate;
        regexes::ResolveGetNodesHistory(Uri, id, startDate, endDate);
        httpServer->GetNodesHistory(id, startDate, endDate, ResponseCode, ResponseMsg);
    }
    else if (std::regex_match(Uri, regexes::Update))
    {
        string date;
        regexes::ResolveUpdate(Uri, date);
        httpServer->Update(date, ResponseCode, ResponseMsg);
    }
    else
    {
        ResponseCode = HTTP_BADREQUEST;
    }
    if (ResponseCode == HTTP_BADREQUEST)
        ResponseMsg = BadRequestResponse();
    if (ResponseCode == HTTP_NOTFOUND)
        ResponseMsg = NotFoundResponse();
    evbuffer_add_printf(OutBuf, "%s", ResponseMsg.data());
    evhttp_send_reply(req, ResponseCode, "", OutBuf);
};

#endif //SERVER_OnRequestUEST_H
