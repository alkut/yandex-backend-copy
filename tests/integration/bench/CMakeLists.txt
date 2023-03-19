project(bench_integration)

add_executable(bench_integration integration_bench.cpp)

add_test(
        NAME bench_integration
        COMMAND bench_integration
)

target_link_libraries(bench_integration PUBLIC benchmark::benchmark)