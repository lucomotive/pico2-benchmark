#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
  auto [mat, time] = alloc::heap<P>(size);

  printf("MATRIX:\n");
  print_float_matrix(mat);
  printf("TIME: %llu\n", time);
  // print_all<'\n'>("MATRIX:", mat.format(EIGEN_FMT), "TIME:", time);
}
