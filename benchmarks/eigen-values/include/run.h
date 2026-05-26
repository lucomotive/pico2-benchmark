#pragma once

#include "benchmarks/benchmarks.h"
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <cstdint>
#include <cstdio>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> inline void op(uint32_t size) {
#if defined(ENABLE_PSRAM)
  auto *s_cache = (uint8_t *)PSRAM_BASE;
  Map<Mat<P>> source((P *)s_cache, size, size);
#else
  Mat<P> source(size, size);
#endif
  source.setRandom();

  EigenSolver<Mat<P>> solver;
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
