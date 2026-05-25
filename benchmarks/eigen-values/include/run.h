#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "repeat.h"
#include "timer.h"
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint32_t size) {
  EigenSolver<Mat<P>> solver;
  const Mat<P> source(Mat<P>::Random(size, size));
  auto time = eigen::eigen(solver, source);
  bool success = solver.info() == Success;
  printf("%u,%llu,%u\n", size, time, success);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us,computable\n");
  const uint16_t min = 5;
  const uint16_t max = 250;
  const uint16_t step = 2;

  for (uint16_t x = min; x <= max; x += (int)step)
    op<P>(x);
}
