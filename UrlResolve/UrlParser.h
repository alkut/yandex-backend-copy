#ifndef YAD_URLPARSER_H
#define YAD_URLPARSER_H

#include "Includes.h"
#include "UrlParametersContainer.h"

class WrongUrlException : std::exception
{
public:
    WrongUrlException() = default;
};

class UrlParser
{
public:
    UrlParser(const string& url, const string& body);

    template<RequestType T>
    RequestParameters<T> ResolveUrl();
private:
    const string Import = "/imports";
    const string Delete = "/delete";
    const string GetNodes = "/nodes";
    const string GetNodesHistory = "/node";
    const string Update = "/updates";
    string _url;
    string _body;
    RequestType _type;
    unordered_map<string, string> _parse(const string& s);
    RequestParameters<RequestType::Import> _resolveImport();
    RequestParameters<RequestType::Delete> _resolveDelete();
    RequestParameters<RequestType::GetNodes> _resolveGetNodes();
    RequestParameters<RequestType::GetNodesHistory> _resolveGetNodesHistory();
    RequestParameters<RequestType::Update> _resolveUpdate();
};

#endif //YAD_URLPARSER_H
