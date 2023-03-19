project(model_lib)

set(SOURCES
        HttpServer.cpp TreeBuilder.cpp)
set(HEADERS
        HttpServer.hpp TreeBuilder.hpp)

add_library(model_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(model_lib PUBLIC Boost::system)