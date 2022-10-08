cmake_minimum_required(VERSION 3.18)
project(yad)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Boost 1.40 REQUIRED)
find_package (glog 0.6.0 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(EVENT REQUIRED libevent)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(yad main.cpp Configuration.h Configuration.cpp View/ImportBody.h UrlResolvers.h Includes.h Model/HttpServer.h OnRequest.h View/GetNodesBody.h GlobalUtils.h Model/FileSystemTree.h Model/HistoryStorage.cpp GlobalUtils.cpp)
target_link_libraries(yad ${Boost_LIBRARIES})
target_include_directories(yad PRIVATE ${EVENT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${SOME_DEP_DIR}/include)
target_link_libraries(yad PUBLIC ${EVENT_LIBRARIES})
target_link_libraries(yad PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries (yad PRIVATE glog::glog)
