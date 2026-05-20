#pragma once

#include "timer.h"
#include "utils.h"
#include <Eigen/src/Core/MatrixBase.h>

namespace benchmarks {
namespace det {

template <typename S>
no_inline std::tuple<Time, S> determinant(const Mat<S> &source) {
  Timer timer;
  S result = source.determinant();
  auto time = timer.elapsed();
  return {time, result};
}

template <typename S, typename Mat>
no_inline std::tuple<Time, S> stack(const Mat &source) {
  Timer timer;
  S result = source.determinant();
  auto time = timer.elapsed();
  return {time, result};
}

}; // namespace det
}; // namespace benchmarks
