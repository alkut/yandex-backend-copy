project(logging_lib)

set(SOURCES
        init.cpp)
set(HEADERS
        init.hpp)

add_library(logging_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(logging_lib glog::glog)