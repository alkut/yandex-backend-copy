#ifndef YAD_URLPARAMETERSCONTAINER_H
#define YAD_URLPARAMETERSCONTAINER_H

#include "Includes.h"
#include "GlobalUtils.h"

enum class RequestType {Import, Delete, GetNodes, GetNodesHistory, Update};

template<RequestType T>
class RequestParameters
{
public:
    RequestParameters() = delete;
};

template<>
class RequestParameters<RequestType::Import>
{
public:
    explicit RequestParameters(const string& s);
    void Validate(CodeMessage& result);
};

template<>
class RequestParameters<RequestType::Delete>
{
public:
    explicit RequestParameters(const string& s);
    void Validate(CodeMessage& result);
    string id;
    string date;
    long long date_ms;
};

template<>
class RequestParameters<RequestType::GetNodes>
{
public:
    explicit RequestParameters(const string& s);
    void Validate(CodeMessage& result);
    string id;
};

template<>
class RequestParameters<RequestType::GetNodesHistory>
{
public:
    explicit RequestParameters(const string& s);
    void Validate(CodeMessage& result);
    string id;
    string dateStart;
    string dateEnd;
    long long dateStart_ms;
    long long dateEnd_ms;
};

template<>
class RequestParameters<RequestType::Update>
{
public:
    explicit RequestParameters(const string& s);
    void Validate(CodeMessage& result);
    string date;
    long long date_ms;
};

#endif //YAD_URLPARAMETERSCONTAINER_H
