#pragma once

#include "utils.h"
#include <Eigen/Householder>
#include <timer.h>

namespace benchmarks {
namespace matmul {
using namespace my_timer;

// template <typename S>
// no_inline Time matmul(const Mat<S> &m1, const Mat<S> &m2, Mat<S> &res) {
//   Timer timer;
//   res.noalias() = m1 * m2;
//   return timer.elapsed_us();
// }

template <typename M1, typename M2, typename R>
no_inline Time matmul(const M1 &m1, const M2 &m2, R &res) {
  Timer timer;
  res.noalias() = m1 * m2;
  return timer.elapsed_us();
}

}; // namespace matmul
}; // namespace benchmarks
