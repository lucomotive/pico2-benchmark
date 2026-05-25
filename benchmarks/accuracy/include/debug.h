#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint16_t size) {
  Mat<P> m1(Mat<P>::Random(size, size));
  Mat<P> m2(Mat<P>::Random(size, size));
  Mat<P> RES;
  auto time = matmul::matmul(m1, m2, RES);

  print_all<'\n'>("RESULT:", RES.format(EIGEN_FMT), "NORM:", RES.norm(),
                  "TIME:", time);
}
