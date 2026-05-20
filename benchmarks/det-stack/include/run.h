#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include "utils.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P, size_t SIZE> inline void op() {
  const Matrix<P, SIZE, SIZE> source(Matrix<P, SIZE, SIZE>::Random());
  auto [time, res] = det::stack<P>(source);
  printf("%u,%llu\n", SIZE, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  const uint16_t min = 5;
  const uint16_t max = 100;
  const uint16_t step = 2;

  repeat<max, step, min>([](auto i) { op<P, i>(); });
}
