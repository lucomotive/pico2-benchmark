#pragma once

#include "const_rand.h"
#include "timer.h"
#include "utils.h"
#include <Eigen/Dense>

namespace benchmarks {
namespace copy {

template <typename S> no_inline Time heap(Mat<S> &res, const Mat<S> &source) {
  Timer timer;
  res = source;
  return timer.elapsed_us();
}

template <typename S, uint16_t SIZE>
no_inline Time stack(Matrix<S, SIZE, SIZE> &res,
                     const Matrix<S, SIZE, SIZE> &source) {
  Timer timer;
  res = source;
  return timer.elapsed_us();
}

}; // namespace copy
}; // namespace benchmarks
