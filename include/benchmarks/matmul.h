#pragma once

#include "utils.h"
#include <Eigen/Householder>
#include <timer.h>

namespace benchmarks {
namespace matmul {

template <typename S>
no_inline Time matmul(const Mat<S> &m1, const Mat<S> &m2, Mat<S> &res) {
  Timer timer;
  res.noalias() = m1 * m2;
  return timer.elapsed_us();
}

template <typename S, typename Mat1, typename Mat2, typename Res>
no_inline Time stack(const Mat1 &m1, const Mat2 &m2, Res &res) {
  Timer timer;
  res.noalias() = m1 * m2;
  return timer.elapsed_us();
}

}; // namespace matmul
}; // namespace benchmarks
