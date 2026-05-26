#pragma once

#include "benchmarks/benchmarks.h"
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

  auto [time, res] = det::determinant<P>(source);

  printf("%u,%llu\n", size, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  const uint16_t min = 5;
  const uint16_t max = 500;
  const uint16_t step = 2;
  for (uint16_t size = min; size <= max; size += (int)step)
    op<P>(size);
}
