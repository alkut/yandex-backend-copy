project(integration_tests)

add_executable(integration_tests integration_test.cpp)

target_link_libraries(integration_tests PUBLIC ${GTEST_LIBRARIES} pthread)