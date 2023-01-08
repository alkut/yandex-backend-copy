cmake_minimum_required(VERSION 3.18)

project(main)
project(runTests)
project(lib)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
if(APPLE)
    link_directories(
            /usr/local/lib
            /usr/lib
    )
    INCLUDE_DIRECTORIES(
            /usr/local/include/
            /usr/include
    )
endif()

FetchContent_Declare(json DOWNLOAD_EXTRACT_TIMESTAMP URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Boost 1.40 REQUIRED)
find_package (glog 0.6.0 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)
pkg_search_module(EVENT REQUIRED libevent)

set(SOURCES Configuration.cpp GlobalUtils.cpp DateExtension.cpp RequestResolver.cpp
        Model/HistoryStorage.cpp
        UrlResolve/UrlParser.cpp UrlResolve/UrlParametersContainer.cpp
        View2/Item.cpp View2/Item.h View2/GetNodeResponse.cpp View2/UpdateResponse.cpp
        Model/FileSystemTree.cpp
        application/Application.cpp)

set(HEADERS Configuration.h UrlResolvers.h Includes.h OnRequest.h GlobalUtils.h DateExtension.h RequestResolver.h
        View/ImportBody.h View/GetNodesBody.h
        Model/HttpServer.h Model/FileSystemTree.h
        UrlResolve/UrlParser.h UrlResolve/UrlParametersContainer.h
        View2/Item.h View2/GetNodeResponse.h View2/GetNodeHistoryResponse.h View2/UpdateResponse.h
        application/Respond.hpp application/QueryResponder.hpp application/EchoServer.hpp application/Application.hpp application/LibeventArgs.hpp)

list(APPEND LINK_DEP ${Boost_LIBRARIES} ${EVENT_LIBRARIES} nlohmann_json::nlohmann_json glog::glog)
list(APPEND INCLUDE_DEP ${EVENT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${SOME_DEP_DIR}/include)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

add_library(lib ${SOURCES} ${HEADERS})

add_executable(main main.cpp)
add_executable(runTests tests/test.cpp)

target_link_libraries(lib ${LINK_DEP})
target_include_directories(lib PUBLIC ${INCLUDE_DEP})

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread lib)
target_link_libraries(main lib)