#pragma once

#include "benchmarks.h"
#include "print.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint32_t rows, uint32_t cols) {
  Mat<P> source(Mat<P>::Random(rows, cols));
  FullPivLU<Mat<P>> lu(rows, cols);
  auto time = lu::full_piv(lu, source);
  printf("%u,%u,%llu\n", rows, cols, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows,cols,time_us\n");
  const uint16_t min = 5;
  const uint16_t max = 500;
  const uint16_t step = 2;
  const float max_ratio = 0.35;
  for (uint16_t x = min; x <= max; x += (int)step) {
    for (uint16_t y = min; y <= x; y += (int)step) {
      if ((float)y < (float)x * max_ratio)
        continue;

      op<P>(x, y);
      if (x != y)
        op<P>(y, x);
    }
  }
}
