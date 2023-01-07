cmake_minimum_required(VERSION 3.18)
project(yad)
project(runTests)
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

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Boost 1.40 REQUIRED)
find_package (glog 0.6.0 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)
pkg_search_module(EVENT REQUIRED libevent)


include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(yad main.cpp Configuration.h Configuration.cpp View/ImportBody.h UrlResolvers.h Includes.h Model/HttpServer.h OnRequest.h View/GetNodesBody.h GlobalUtils.h Model/FileSystemTree.h Model/HistoryStorage.cpp GlobalUtils.cpp UrlResolve/UrlParser.cpp UrlResolve/UrlParser.h UrlResolve/UrlParametersContainer.cpp UrlResolve/UrlParametersContainer.h DateExtension.cpp DateExtension.h View2/Item.cpp View2/Item.h View2/GetNodeResponse.cpp View2/GetNodeResponse.h View2/GetNodeHistoryResponse.h View2/UpdateResponse.cpp View2/UpdateResponse.h RequestResolver.cpp RequestResolver.h Model/FileSystemTree.cpp application/Application.cpp application/Respond.hpp application/QueryResponder.hpp application/EchoServer.hpp application/Application.hpp application/LibeventArgs.hpp)
add_executable(runTests tests/test.cpp Configuration.h Configuration.cpp View/ImportBody.h UrlResolvers.h Includes.h Model/HttpServer.h OnRequest.h View/GetNodesBody.h GlobalUtils.h Model/FileSystemTree.h Model/HistoryStorage.cpp GlobalUtils.cpp UrlResolve/UrlParser.cpp UrlResolve/UrlParser.h UrlResolve/UrlParametersContainer.cpp UrlResolve/UrlParametersContainer.h DateExtension.cpp DateExtension.h View2/Item.cpp View2/Item.h View2/GetNodeResponse.cpp View2/GetNodeResponse.h View2/GetNodeHistoryResponse.h View2/UpdateResponse.cpp View2/UpdateResponse.h RequestResolver.cpp RequestResolver.h Model/FileSystemTree.cpp application/Application.cpp application/Respond.hpp application/QueryResponder.hpp application/EchoServer.hpp application/Application.hpp application/LibeventArgs.hpp)
target_link_libraries(yad ${Boost_LIBRARIES})
target_include_directories(yad PRIVATE ${EVENT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${SOME_DEP_DIR}/include)
target_link_libraries(yad PUBLIC ${EVENT_LIBRARIES})
target_link_libraries(yad PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries (yad PRIVATE glog::glog)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
