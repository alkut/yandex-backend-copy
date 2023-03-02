#ifndef YAD_RESPOND_HPP
#define YAD_RESPOND_HPP
#include <string>

#define HTTP_OK 200
#define HTTP_NOTFOUND 404
#define HTTP_BADREQUEST 400

namespace yad_server::application{
    struct Respond {
        int code = HTTP_OK;
        std::string message;
    };
}
#endif //YAD_RESPOND_HPP
