#ifndef YAD_LIBEVENTARGS_HPP
#define YAD_LIBEVENTARGS_HPP
#include "QueryResponder.hpp"
namespace yad_server::application {
        struct LibeventArgs {
            QueryResponder *responder;
            std::function<void(void)> stop_callback;
        };
    }
#endif //YAD_LIBEVENTARGS_HPP