project(logging_lib)

find_package(glog 0.6.0 REQUIRED)

set(SOURCES
        init.cpp)
set(HEADERS
        init.hpp)

add_library(logging_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(logging_lib glog::glog)