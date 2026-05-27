#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint32_t x, uint32_t y, uint32_t z> void debug() {
  Matrix<P, x, y> M1;
  Matrix<P, y, z> M2;
  Matrix<P, x, z> RES;

  M1.setRandom();
  M2.setRandom();

  auto time = matmul::matmul(M1, M2, RES);

  printf("M1:\n");
  print_float_matrix(M1);

  printf("M2:\n");
  print_float_matrix(M2);

  printf("RES:\n");
  print_float_matrix(RES);

  printf("TIME: %llu\n", time);

  // print_all<'\n'>("M1:", M1.format(EIGEN_FMT), "M2:", M2.format(EIGEN_FMT),
  //                 "RES:", RES.format(EIGEN_FMT), "TIME:", time);
}
