#pragma once

#include "const_rand.h"
#include "utils.h"
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <timer.h>

namespace benchmarks {
namespace eigen {

template <typename S>
no_inline Time eigen(EigenSolver<Mat<S>> &solver, const Mat<S> &A) {
  Timer timer;
  solver.compute(A);
  return timer.elapsed_us();
}

}; // namespace eigen
}; // namespace benchmarks
