project(bench_unit)

find_package(benchmark REQUIRED)

add_executable(bench_unit unit_bench.cpp)

target_link_libraries(bench_unit PUBLIC benchmark::benchmark)