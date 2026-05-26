#pragma once

#include "utils.h"
#include <timer.h>

namespace benchmarks {
namespace rank {
using namespace my_timer;

template <typename Solver>
no_inline std::tuple<Time, int> rank(const Solver &lu) {
  Timer timer;
  auto rank = lu.rank();
  return {timer.elapsed_us(), rank};
}

}; // namespace rank
}; // namespace benchmarks
