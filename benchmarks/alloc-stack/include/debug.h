#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint32_t SIZE> void debug() {
  Eigen::Matrix<P, SIZE, SIZE> res;
  auto time = alloc::stack<P, SIZE>(res);

  print_all<'\n'>("SOURCE:", res.format(EIGEN_FMT), "TIME:", time);
}
