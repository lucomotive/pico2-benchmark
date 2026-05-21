#pragma once

#include "utils.h"
#include <timer.h>

namespace benchmarks {
namespace inverse {

template <typename S>
no_inline Time inverse(Mat<S> &result, const Mat<S> &source) {
  Timer timer;
  result = source.inverse();
  return timer.elapsed_us();
}

}; // namespace inverse
}; // namespace benchmarks
