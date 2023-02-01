project(unit_tests)

find_package(GTest REQUIRED)

add_executable(unit_tests unit_test.cpp)

target_link_libraries(unit_tests PUBLIC ${GTEST_LIBRARIES} pthread)