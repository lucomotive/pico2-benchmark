#pragma once

#include "benchmarks/benchmarks.h"
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
  source.setRandom();

  FullPivLU<Mat<P>> lu(source);
  auto [time, rank] = rank::rank(lu);

  printf("%u,%u,%llu,%u\n", rows, cols, time, rank);
};

template <typename P> void run() {
  // run benchmark
  printf("rows,cols,time_us,rank\n");
  constexpr uint16_t min = 5;
  constexpr uint16_t max = 500;
  constexpr uint16_t step = 2;
  constexpr float max_ratio = 0.4;
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
