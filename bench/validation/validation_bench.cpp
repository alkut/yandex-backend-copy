#include "bench/validation/validation_bench.hpp"

void BM_DateToPosix(benchmark::State& state) {
    const std::string sample = "2000-01-01 00:00:00.000";
    for ([[maybe_unused]] const auto _ : state) {
        yad_server::validation::Validator::check_datetime(sample);
    }
}
