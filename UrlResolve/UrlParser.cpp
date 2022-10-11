#include "UrlParser.h"

UrlParser::UrlParser(const string &url, const string &body) : _url(url), _body(body)
{
    if (url.starts_with(Import))
    {
        _type = RequestType::Import;
        return;
    }

    if (url.starts_with(Delete))
    {
        _type = RequestType::Delete;
        return;
    }

    if (url.starts_with(GetNodes))
    {
        _type = RequestType::GetNodes;
        return;
    }

    if (url.starts_with(GetNodesHistory))
    {
        _type = RequestType::GetNodesHistory;
        return;
    }

    if (url.starts_with(Update))
    {
        _type = RequestType::Update;
        return;
    }

    throw WrongUrlException();
}

template<RequestType T>
RequestParameters<T> UrlParser::ResolveUrl() {
    switch (_type)
    {
        case RequestType::Import :
            return _resolveImport();
        case RequestType::Delete :
            return _resolveDelete();
        case RequestType::GetNodes :
            return _resolveGetNodes();
        case RequestType::GetNodesHistory :
            return _resolveGetNodesHistory();
        case RequestType::Update :
            return _resolveUpdate();
    }
}
