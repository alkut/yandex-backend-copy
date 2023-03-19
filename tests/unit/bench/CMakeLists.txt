project(bench_unit)

add_executable(bench_unit unit_bench.cpp)

add_test(
        NAME bench_unit
        COMMAND bench_unit
)

target_link_libraries(bench_unit PUBLIC benchmark::benchmark)