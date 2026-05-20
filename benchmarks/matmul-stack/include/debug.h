#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t x, uint32_t y, uint32_t z) {
  Mat<P> M1(Mat<P>::Random(x, y));
  Mat<P> M2(Mat<P>::Random(y, z));
  Mat<P> RES;
  auto time = matmul::matmul(M1, M2, RES);

  print_all<'\n'>("M1:", M1.format(EIGEN_FMT), "M2:", M2.format(EIGEN_FMT),
                  "RES:", RES.format(EIGEN_FMT), "TIME:", time);
}
