// Copyright 2023 Alexsey Kutasov
#ifndef SRC_APPLICATION_QUERYRESPONDER_HPP_
#define SRC_APPLICATION_QUERYRESPONDER_HPP_
#include "src/application/Respond.hpp"
#include "src/application/Query.hpp"

class QueryResponder {
 public:
    virtual Respond Response(const query::Query& query) = 0;
};

#endif  // SRC_APPLICATION_QUERYRESPONDER_HPP_
