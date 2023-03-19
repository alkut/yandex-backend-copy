project(unit_tests)

add_executable(unit_tests unit_test.cpp)

add_test(
        NAME unit_tests
        COMMAND unit_tests
)

target_link_libraries(unit_tests PUBLIC GTest::GTest pthread)