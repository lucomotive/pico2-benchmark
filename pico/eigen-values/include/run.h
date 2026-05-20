#pragma once

#include "benchmarks.h"
#include "benchmarks/eigen.h"
#include "print.h"
#include "utils.h"
#include <Eigen/Dense>
#include <Eigen/src/Core/util/Constants.h>
#include <Eigen/src/Eigenvalues/EigenSolver.h>
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint32_t rows, uint32_t cols) {
  EigenSolver<Mat<P>> solver(rows, cols);
  Mat<P> source(Mat<P>::Random(rows, cols));
  auto time = eigen::eigen(solver, source);
  bool success = solver.info() == Success;
  printf("%u,%u,%llu,%u\n", rows, cols, time, success);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us,computable\n");
  const uint16_t min = 5;
  const uint16_t max = 500;
  const uint16_t step = 2;

  repeat<max, step, min>(op<P>);
}
