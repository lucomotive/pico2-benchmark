#pragma once

#include "timer.h"
#include "utils.h"
#include <Eigen/src/Core/MatrixBase.h>

namespace benchmarks {
namespace det {
using namespace my_timer;

// template <typename S>
// no_inline std::tuple<Time, S> determinant(const Map<MatrixBase<S>> &source) {
//   Timer timer;
//   S result = source.determinant();
//   return {timer.elapsed_us(), result};
// }

template <typename S, typename Mat>
no_inline std::tuple<Time, S> determinant(const Mat &source) {
  Timer timer;
  S result = source.determinant();
  return {timer.elapsed_us(), result};
}

}; // namespace det
}; // namespace benchmarks
