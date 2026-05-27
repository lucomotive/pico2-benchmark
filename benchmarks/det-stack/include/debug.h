#pragma once

#include "benchmarks/benchmarks.h"
#include "benchmarks/det.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, size_t SIZE> void debug() {
  Matrix<P, SIZE, SIZE> source;
  source.setRandom();

  auto [time, res] = det::determinant<P>(source);

  printf("SOURCE:\n");
  print_float_matrix(source);
  printf("DETERMINANT: %.17g\n", res);
  printf("TIME: %llu\n", time);

  // print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "DETERMINANT:", res,
  //                 "TIME:", time);
}
