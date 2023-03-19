project(integration_tests)

add_executable(integration_tests integration_test.cpp)

add_test(
        NAME integration_tests
        COMMAND integration_tests
)

target_link_libraries(integration_tests PUBLIC GTest::GTest pthread)