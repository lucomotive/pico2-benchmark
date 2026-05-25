#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint32_t rows, uint32_t cols) {
  const Mat<P> source(Mat<P>::Random(rows, cols));
  FullPivLU<Mat<P>> lu(rows, cols);
  const auto time = lu::full_piv(lu, source);
  printf("%u,%u,%llu\n", rows, cols, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows,cols,time_us\n");
  constexpr uint16_t min = 5;
  constexpr uint16_t max = 250;
  constexpr uint16_t step = 2;
  constexpr float max_ratio = 0.35;
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
