project(view_lib)

set(SOURCES
        GetNodesBody.cpp ImportBody.cpp)
set(HEADERS
        GetNodesBody.hpp ImportBody.hpp)

add_library(view_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(view_lib PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(view_lib PUBLIC ${SOME_DEP_DIR}/include)