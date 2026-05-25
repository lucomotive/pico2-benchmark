#pragma once

#include "benchmarks/benchmarks.h"
#include "repeat.h"
#include <Eigen/Dense>
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P, uint16_t SIZE> inline void op() {
  auto [mat, time] = alloc::stack<P, SIZE>();
  printf("%u,%llu\n", SIZE, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  constexpr uint16_t min = 5;
  // a matrix of size 150x150 and type float seems to overflow the stack
  // 4 * 150 * 150 = s * y^2
  // where s is the data type size (4 or 8) and y is the max dimension
  // y = 300 / sqrt(s)
  // y(4) = 150
  // y(8) = 106
  constexpr uint16_t max = 300 / std::sqrt((P)sizeof(P));
  constexpr uint16_t step = 2;

  repeat<max, step, min>([](auto i) { op<P, i>(); });
}
