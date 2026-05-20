#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint32_t x, uint32_t y, uint32_t z> void debug() {
  Matrix<P, x, y> M1(Matrix<P, x, y>::Random());
  Matrix<P, y, z> M2(Matrix<P, y, z>::Random());
  Matrix<P, x, z> RES;
  auto time = matmul::stack<P>(M1, M2, RES);

  print_all<'\n'>("M1:", M1.format(EIGEN_FMT), "M2:", M2.format(EIGEN_FMT),
                  "RES:", RES.format(EIGEN_FMT), "TIME:", time);
}
