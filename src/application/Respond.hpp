#ifndef YAD_RESPOND_HPP
#define YAD_RESPOND_HPP
#include <string>
#include <event2/http.h>

struct Respond {
    int code = HTTP_OK;
    std::string message;
};
#endif //YAD_RESPOND_HPP
