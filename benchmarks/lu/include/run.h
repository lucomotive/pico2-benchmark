#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <cstdint>
#include <cstdio>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> inline void op(uint32_t rows, uint32_t cols) {
#if defined(ENABLE_PSRAM)
  auto *s_cache = (uint8_t *)PSRAM_BASE;
  Map<Mat<P>> source((P *)s_cache, rows, cols);
#else
  Mat<P> source(rows, cols);
#endif

  FullPivLU<Mat<P>> lu(rows, cols);
  const auto time = lu::full_piv(lu, source);

  printf("%u,%u,%llu\n", rows, cols, time);
};

template <typename P>
void loop(uint16_t min, uint16_t max, uint16_t step, float max_ratio) {
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

template <typename P> void run() {
  // run benchmark
  printf("rows,cols,time_us\n");
  loop<P>(5, 25, 2, 0.2);
  loop<P>(25, 50, 3, 0.3);
  loop<P>(50, 100, 5, 0.4);
  loop<P>(100, 200, 8, 0.5);
  loop<P>(200, 500, 15, 0.5);
}
