#pragma once

#include "const_rand.h"
#include "timer.h"
#include "utils.h"
#include <Eigen/Dense>

namespace benchmarks {
namespace copy {
using namespace my_timer;

template <typename Res, typename Source>
no_inline Time copy(Res &res, const Source &source) {
  Timer timer;
  res = source;
  return timer.elapsed_us();
}

// template <typename S, uint16_t SIZE>
// no_inline Time stack(Matrix<S, SIZE, SIZE> &res,
//                      const Matrix<S, SIZE, SIZE> &source) {
//   Timer timer;
//   res = source;
//   return timer.elapsed_us();
// }

}; // namespace copy
}; // namespace benchmarks
