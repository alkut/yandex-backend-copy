project(bench_unit)

add_executable(bench_unit unit_bench.cpp)

target_link_libraries(bench_unit PUBLIC benchmark::benchmark)