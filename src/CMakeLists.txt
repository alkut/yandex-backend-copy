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
find_package(glog 0.6.0 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)
find_package(CURL)
pkg_search_module(EVENT REQUIRED libevent)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/bin")


set(SOURCES
        Model/HistoryStorage.cpp Model/FileSystemTree.cpp Model/HttpServer.cpp
        application/Application.cpp
        validation/QueryExt.cpp validation/Validator.cpp
        logging/init.cpp)

set(HEADERS
        View/ImportBody.h View/GetNodesBody.h
        Model/HttpServer.hpp Model/FileSystemTree.hpp
        application/Respond.hpp application/QueryResponder.hpp application/Application.hpp application/LibeventArgs.hpp
        validation/QueryExt.hpp validation/Validator.hpp validation/DeleteItem.hpp
        logging/init.hpp)

set(TEST_SOURCES ../tests/EchoServer.cpp)
set(TEST_HEADERS ../tests/EchoServer.hpp)


list(APPEND LINK_DEP ${Boost_LIBRARIES} ${EVENT_LIBRARIES} nlohmann_json::nlohmann_json glog::glog)
list(APPEND INCLUDE_DEP ${EVENT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${SOME_DEP_DIR}/include)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIR})

add_library(lib ${SOURCES} ${HEADERS})

add_executable(main main.cpp)
add_executable(runTests ../tests/test.cpp ${TEST_SOURCES} ${TEST_HEADERS})

target_link_libraries(lib ${LINK_DEP})
target_include_directories(lib PUBLIC ${INCLUDE_DEP})

target_link_libraries(runTests ${CURL_LIBRARIES} ${GTEST_LIBRARIES} pthread lib )
target_link_libraries(main lib)