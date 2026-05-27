#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint32_t SIZE> void debug() {
  auto [mat, time] = alloc::stack<P, SIZE>();

  printf("MATRIX:\n");
  print_float_matrix(mat);
  printf("TIME: %llu\n", time);
  // print_all<'\n'>("MATRIX:", mat.format(EIGEN_FMT), "TIME:", time);
}
