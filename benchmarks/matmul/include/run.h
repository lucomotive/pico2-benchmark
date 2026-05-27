#pragma once

#include "benchmarks/benchmarks.h"
#include "my_rand.h"
#include <cstdint>
#include <cstdio>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> inline void op(uint32_t x, uint32_t y, uint32_t z) {

#if defined(ENABLE_PSRAM)
  auto *m1_cache = (uint8_t *)PSRAM_BASE;
  auto *m2_cache = m1_cache + (sizeof(P) * x * y);
  auto *r_cache = m2_cache + (sizeof(P) * x * z);
  Map<Mat<P>> M1((P *)m1_cache, x, y);
  Map<Mat<P>> M2((P *)m2_cache, y, z);
  Map<Mat<P>> R((P *)r_cache, x, z);
#else
  Mat<P> M1(x, y);
  Mat<P> M2(y, z);
  Mat<P> R;
#endif

  M1.setRandom();
  M2.setRandom();

  const auto time = matmul::matmul(M1, M2, R);
  printf("%u,%u,%u,%llu\n", x, y, z, time);
};

template <typename P>
void loop(uint16_t min, uint16_t max, uint16_t step, float max_ratio) {
  for (uint16_t x = min; x <= max; x += step) {
    for (uint16_t y = min; y <= x; y += step) {
      for (uint16_t z = min; z <= y; z += step) {
        if ((float)y < (float)x * max_ratio ||
            (float)z < (float)y * max_ratio || (float)z < (float)x * max_ratio)
          continue;

        op<P>(x, y, z);
        if (x != y)
          op<P>(y, x, z);
        if (y != z)
          op<P>(x, z, y);
        if (z != x)
          op<P>(z, y, x);
      }
    }
  }
}

template <typename P> void run() {
  // run benchmark
  printf("x,y,z,time_us\n");

  loop<P>(5, 25, 2, 0.2);
  loop<P>(25, 50, 3, 0.3);
  loop<P>(50, 100, 5, 0.4);
  loop<P>(100, 200, 8, 0.5);
  loop<P>(200, 500, 15, 0.5);
}
