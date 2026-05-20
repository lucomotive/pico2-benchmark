#pragma once

#include "timer.h"
#include "utils.h"
#include <Eigen/Householder>

namespace benchmarks {
namespace lu {

template <typename S>
no_inline Time full_piv(FullPivLU<Mat<S>> &lu, const Mat<S> &source) {
  Timer timer;
  lu.compute(source);
  auto time = timer.elapsed();
  return time;
}

}; // namespace lu
}; // namespace benchmarks
