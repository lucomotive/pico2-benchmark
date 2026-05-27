#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint32_t SIZE> void debug() {
  Matrix<P, SIZE, SIZE> source;
  source.setRandom();
  Matrix<P, SIZE, SIZE> res;
  auto time = copy::copy(res, source);

  printf("SOURCE:\n");
  print_float_matrix(source);
  printf("COPY:\n");
  print_float_matrix(res);
  printf("TIME: %llu\n", time);

  // print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
  //                 "COPY:", res.format(EIGEN_FMT), "TIME:", time);
}
