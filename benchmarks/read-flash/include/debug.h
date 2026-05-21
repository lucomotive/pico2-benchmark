#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint16_t SIZE> void debug() {
  Mat<P> res;
  auto time = alloc::from_flash<P, SIZE>(res);

  print_all<'\n'>("RESULT:", res.format(EIGEN_FMT), "TIME:", time);
}
