project(bench_integration)

add_executable(bench_integration integration_bench.cpp)

target_link_libraries(bench_integration PUBLIC benchmark::benchmark)