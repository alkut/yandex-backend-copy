#ifndef YAD_QUERYRESPONDER_HPP
#define YAD_QUERYRESPONDER_HPP
#include "Respond.hpp"
#include "Query.hpp"

class QueryResponder {
public:
    virtual Respond Response(const Query& query) = 0;
};

#endif //YAD_QUERYRESPONDER_HPP
