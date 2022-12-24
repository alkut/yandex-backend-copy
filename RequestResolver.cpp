#include "RequestResolver.h"
#include "UrlResolve/UrlParser.h"

/*
RequestResolver::RequestResolver(evhttp_request *request, HttpServer *server) : _request(request), _server(server)
{
    _readInput();
    UrlParser parser(_request->uri, _inputMessage);
    //auto parameters = parser.ResolveUrl();
    //_server->exexute(parameters);
}

void RequestResolver::_readInput()
{
    _inputMessage.clear();
    char buf[maxMessageLenght];
    struct evbuffer* buffer = evhttp_request_get_input_buffer(_request);
    int n = 0;
    while ((n = evbuffer_remove(buffer, buf, 4096)) > 0)
    {
        for (int i = 0; i < n; ++i)
            _inputMessage.push_back(buf[i]);
    }
}*/
