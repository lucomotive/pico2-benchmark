#pragma once

#include "const_rand.h"
#include "utils.h"
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <timer.h>

namespace benchmarks {
namespace eigen {
using namespace my_timer;

template <typename Solver, typename Mat>
no_inline Time eigen(Solver &solver, const Mat &A) {
  Timer timer;
  solver.compute(A);
  return timer.elapsed_us();
}

}; // namespace eigen
}; // namespace benchmarks
