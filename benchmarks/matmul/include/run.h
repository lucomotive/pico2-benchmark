#pragma once

#include "benchmarks/benchmarks.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint32_t x, uint32_t y, uint32_t z) {
  Mat<P> M1(Mat<P>::Random(x, y));
  Mat<P> M2(Mat<P>::Random(y, z));
  Mat<P> RES;
  auto time = matmul::matmul(M1, M2, RES);
  printf("%u,%u,%u,%llu\n", y, x, z, time);
};

template <typename P> void run() {
  // run benchmark
  printf("x,y,z,time_us\n");
  const uint16_t min = 5;
  const uint16_t max = 250;
  const uint16_t step = 3;
  const float max_ratio = 0.35;
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
