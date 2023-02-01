project(view_lib)

FetchContent_Declare(json DOWNLOAD_EXTRACT_TIMESTAMP URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

set(SOURCES
        Query.cpp BoostApplication.cpp)
set(HEADERS
        Respond.hpp QueryResponder.hpp Query.hpp BoostApplication.hpp)

add_library(view_lib ${SOURCES} ${HEADERS})

target_link_libraries(view_lib nlohmann_json::nlohmann_json)
target_include_directories(view_lib PUBLIC ${SOME_DEP_DIR}/include)