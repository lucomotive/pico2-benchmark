#pragma once

#include "benchmarks/benchmarks.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> no_inline void op(const Mat<P> &m1, const Mat<P> &m2) {
  Mat<P> RES;
  auto time = matmul::matmul(m1, m2, RES);
  printf("%f,%llu\n", RES.norm(), time);
};

template <typename P> void run() {
  // run benchmark
  printf("norm,time_us\n");
  const uint16_t iterations = 1000;

  const uint16_t SIZE = 50;
  const Mat<P> m1(Mat<P>::Random(SIZE, SIZE));
  const Mat<P> m2(Mat<P>::Random(SIZE, SIZE));
  for (uint16_t i = 0; i < iterations; i++)
    op<P>(m1, m2);
}
