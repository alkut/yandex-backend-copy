add_subdirectory(integration)
add_subdirectory(unit)
add_subdirectory(utils)

target_link_libraries(unit_tests PUBLIC test_util_lib)
target_link_libraries(integration_tests PUBLIC test_util_lib)
target_link_libraries(bench_unit PUBLIC test_util_lib)
target_link_libraries(bench_integration PUBLIC test_util_lib)

