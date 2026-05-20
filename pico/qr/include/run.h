#pragma once

#include "benchmarks.h"
#include "benchmarks/inverse.h"
#include "benchmarks/qr.h"
#include "print.h"
#include <Eigen/src/QR/HouseholderQR.h>
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint32_t rows, uint32_t cols) {
  const Mat<P> source(Mat<P>::Random(rows, cols));
  HouseholderQR<Mat<P>> qr(rows, cols);
  auto time = qr::householder(qr, source);
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
