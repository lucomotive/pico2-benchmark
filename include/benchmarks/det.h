#pragma once

#include "timer.h"
#include "utils.h"

namespace benchmarks {
namespace det {

template <typename S>
no_inline std::tuple<Time, S> determinant(const Mat<S> &source) {
  Timer timer;
  S result = source.determinant();
  auto time = timer.elapsed();
  return {time, result};
}

}; // namespace det
}; // namespace benchmarks
