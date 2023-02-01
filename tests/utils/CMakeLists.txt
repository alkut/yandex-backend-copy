project(test_util_lib)

find_package(CURL REQUIRED)

set(SOURCES
        curl/ExecuteCurl.cpp
        echo/EchoServer.cpp)

set(HEADERS
        curl/ExecuteCurl.hpp
        echo/EchoServer.hpp)

add_library(test_util_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(test_util_lib PUBLIC ${CURL_LIBRARIES})