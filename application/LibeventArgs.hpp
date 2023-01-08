#ifndef YAD_LIBEVENTARGS_HPP
#define YAD_LIBEVENTARGS_HPP
#include "QueryResponder.hpp"

///class to provide callback function for
///OnRequest2 function
struct LibeventArgs {
    QueryResponder *responder;
    std::function <void(void)> stop_callback;
};
#endif //YAD_LIBEVENTARGS_HPP
