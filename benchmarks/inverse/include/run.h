#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint32_t size) {
  const Mat<P> source(Mat<P>::Random(size, size));
  Mat<P> res;
  auto time = inverse::inverse(res, source);

  printf("%u,%lu\n", size, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  const uint16_t min = 5;
  const uint16_t max = 250;
  const uint16_t step = 2;
  for (uint16_t size = min; size <= max; size += (int)step)
    op<P>(size);
}
