// Copyright 2023 Alexsey Kutasov
#ifndef SRC_APPLICATION_LIBEVENTARGS_HPP_
#define SRC_APPLICATION_LIBEVENTARGS_HPP_
#include "src/application/QueryResponder.hpp"

struct LibeventArgs {
    QueryResponder *responder;
    std::function <void(void)> stop_callback;
};
#endif  // SRC_APPLICATION_LIBEVENTARGS_HPP_
