#pragma once

#include "benchmarks/benchmarks.h"
#include "benchmarks/det.h"
#include "print.h"
#include "repeat.h"
#include "timer.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P, uint32_t SIZE> inline void op() {
  Matrix<P, SIZE, SIZE> source;
  source.setRandom();
  auto [time, res] = det::determinant<P>(source);
  printf("%u,%llu\n", SIZE, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  const uint16_t min = 5;
  const uint16_t max = 100;
  const uint16_t step = 2;

  repeat<max, step, min>([](const auto i) { op<P, i>(); });
}
