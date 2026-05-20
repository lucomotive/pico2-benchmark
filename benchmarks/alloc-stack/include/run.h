#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include "utils.h"
#include <Eigen/Dense>
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <uint32_t SIZE, typename P> inline void op() {
  Matrix<P, SIZE, SIZE> res;
  auto time = alloc::stack<P, SIZE>(res);
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

  repeat<max, step, min>([](auto i) { op<i, P>(); });
}
