cmake_minimum_required(VERSION 3.18)

project(runTests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests test.cpp ../application/Application.cpp ../Configuration.hg)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)