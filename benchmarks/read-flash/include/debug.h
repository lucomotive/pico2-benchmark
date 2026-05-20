#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
  Mat<P> res;
  auto time = alloc::from_flash(res, size, size);

  print_all<'\n'>("RESULT:", res.format(EIGEN_FMT), "TIME:", time);
}
