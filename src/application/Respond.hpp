// Copyright 2023 Alexsey Kutasov
#ifndef SRC_APPLICATION_RESPOND_HPP_
#define SRC_APPLICATION_RESPOND_HPP_
#include <event2/http.h>
#include <string>

struct Respond {
    int code = HTTP_OK;
    std::string message;
};
#endif  // SRC_APPLICATION_RESPOND_HPP_

