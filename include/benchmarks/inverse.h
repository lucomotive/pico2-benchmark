#pragma once

#include "utils.h"
#include <timer.h>

namespace benchmarks {
namespace inverse {

template <typename S>
no_inline Time inverse(Mat<S> &result, const Mat<S> &source) {
  Timer timer;
  result = source.inverse();
  auto time = timer.elapsed();
  return time;
}

}; // namespace inverse
}; // namespace benchmarks
