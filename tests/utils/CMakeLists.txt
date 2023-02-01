project(test_util_lib)

set(SOURCES
        curl/ExecuteCurl.cpp
        echo/EchoServer.cpp)

set(HEADERS
        curl/ExecuteCurl.hpp
        echo/EchoServer.hpp)

add_library(test_util_lib STATIC ${SOURCES} ${HEADERS})