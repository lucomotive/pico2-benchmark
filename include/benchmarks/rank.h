#pragma once

#include "utils.h"
#include <timer.h>

namespace benchmarks {
namespace rank {

template <typename S>
no_inline std::tuple<Time, int> rank(const FullPivLU<Mat<S>> &lu) {
  Timer timer;
  auto rank = lu.rank();
  return {timer.elapsed_us(), rank};
}

}; // namespace rank
}; // namespace benchmarks
