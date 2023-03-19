project(unit_tests)

add_executable(unit_tests unit_test.cpp)

target_link_libraries(unit_tests PUBLIC GTest::GTest pthread)