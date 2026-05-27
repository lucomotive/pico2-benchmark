#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint16_t SIZE> void debug() {
  auto [res, time] = alloc::from_flash_stack<P, SIZE>();

  printf("RESULT:\n");
  print_float_matrix(res);
  printf("TIME: %llu\n", time);

  // print_all<'\n'>("RESULT:", res.format(EIGEN_FMT), "TIME:", time);
}
