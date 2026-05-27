#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint16_t size) {
  const Mat<P> m1(Mat<P>::Random(size, size));
  const Mat<P> m2(Mat<P>::Random(size, size));
  Mat<P> RES;
  const auto time = matmul::matmul(m1, m2, RES);

  printf("RESULT:\n");
  print_float_matrix(RES);

  printf("NORM: %.17g\n", RES.norm());
  printf("TIME: %llu\n", time);

  // print_all<'\n'>("RESULT:", RES.format(EIGEN_FMT), "NORM:", RES.norm(),
  //                 "TIME:", time);
}
