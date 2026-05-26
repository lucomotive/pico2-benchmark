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
  auto *t_cache = s_cache + (sizeof(P) * size * size);
  Map<Mat<P>> source((P *)s_cache, size, size);
  Map<Mat<P>> temp((P *)t_cache, size, size);
#else
  Mat<P> source(rows, cols);
  Mat<P> temp;
#endif
  source.setRandom();

  temp = source * source.adjoint();

  LLT<Mat<P>> llt(temp);
  auto time = llt::llt(llt, temp);
  printf("%u,%llu\n", size, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  constexpr uint16_t min = 5;
  constexpr uint16_t max = 500;
  constexpr uint16_t step = 3;
  for (uint16_t x = min; x <= max; x += (int)step)
    op<P>(x);
}
