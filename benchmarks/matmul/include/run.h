#pragma once

#include "benchmarks/benchmarks.h"
#include "ps_ram.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

using namespace benchmarks;

template <typename P> inline void op(uint32_t x, uint32_t y, uint32_t z) {

  // #if defined(ENABLE_PSRAM)
  //   auto *m1_cache = (uint8_t *)psram::BASE_ADDRESS;
  //   auto *m2_cache = m1_cache + (sizeof(P) * x * y);
  //   auto *r_cache = m2_cache + (sizeof(P) * x * z);
  //   Map<Mat<P>> M1((P *)m1_cache, x, y);
  //   Map<Mat<P>> M2((P *)m2_cache, y, z);
  //   Map<Mat<P>> R((P *)r_cache, x, z);
  // #else
  Mat<P> M1(x, y);
  Mat<P> M2(y, z);
  Mat<P> R;
  // #endif

  M1.setRandom();
  M2.setRandom();

  const auto time = matmul::matmul(M1, M2, R);
  printf("%u,%u,%u,%llu\n", y, x, z, time);
};

template <typename P> void run() {
  // run benchmark
  printf("x,y,z,time_us\n");
  constexpr uint16_t min = 5;
  constexpr uint16_t max = 400;
  constexpr uint16_t step = 50;
  constexpr float max_ratio = 0.5;
  for (uint16_t x = min; x <= max; x += (uint16_t)step) {
    for (uint16_t y = min; y <= x; y += (uint16_t)step) {
      for (uint16_t z = min; z <= y; z += (uint16_t)step) {
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
