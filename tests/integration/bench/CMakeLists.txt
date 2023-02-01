project(bench_integration)

find_package(benchmark REQUIRED)

add_executable(bench_integration integration_bench.cpp)

target_link_libraries(bench_integration PUBLIC benchmark::benchmark)