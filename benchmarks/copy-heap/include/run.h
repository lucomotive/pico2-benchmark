#pragma once

#include "benchmarks/benchmarks.h"
#include <Eigen/Dense>
#include <cstdint>
#include <cstdio>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> inline void op(uint32_t size) {
#if defined(ENABLE_PSRAM)
  auto *s_cache = (uint8_t *)PSRAM_BASE;
  auto *r_cache = s_cache + (sizeof(P) * size * size);
  Map<Mat<P>> source((P *)s_cache, size, size);
  Map<Mat<P>> res((P *)r_cache, size, size);
#else
  Mat<P> source(size, size);
  Mat<P> res;
#endif
  source.setRandom();
  res.setRandom();

  auto time = copy::copy(res, source);

  printf("%u,%llu\n", size, time);
};

template <typename P> void loop(uint16_t min, uint16_t max, uint16_t step) {
  for (uint16_t size = min; size <= max; size += (int)step)
    op<P>(size);
}

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  loop<P>(5, 500, 3);
}
