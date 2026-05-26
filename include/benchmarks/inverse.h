#pragma once

#include "utils.h"
#include <timer.h>

namespace benchmarks {
namespace inverse {
using namespace my_timer;

template <typename Mat> no_inline Time inverse(Mat &result, const Mat &source) {
  Timer timer;
  result = source.inverse();
  return timer.elapsed_us();
}

}; // namespace inverse
}; // namespace benchmarks
