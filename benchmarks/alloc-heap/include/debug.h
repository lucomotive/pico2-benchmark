#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
  auto [mat, time] = alloc::heap<P>(size);
  print_all<'\n'>("MATRIX:", mat.format(EIGEN_FMT), "TIME:", time);
}
