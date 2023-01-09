#ifndef YAD_LIBEVENTARGS_HPP
#define YAD_LIBEVENTARGS_HPP
#include "QueryResponder.hpp"

struct LibeventArgs {
    QueryResponder *responder;
    std::function <void(void)> stop_callback;
};
#endif //YAD_LIBEVENTARGS_HPP