#pragma once

#include "benchmarks/benchmarks.h"
#include "repeat.h"
#include <Eigen/Dense>
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P, uint32_t SIZE> inline void op() {
  const Matrix<P, SIZE, SIZE> source(Matrix<P, SIZE, SIZE>::Random(SIZE, SIZE));
  Matrix<P, SIZE, SIZE> res;
  auto time = copy::stack<P, SIZE>(res, source);
  printf("%u,%lu\n", SIZE, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  constexpr uint16_t min = 5;
  constexpr uint16_t max = 100;
  constexpr uint16_t step = 2;

  repeat<max, step, min>([](auto i) { op<P, i>(); });
}
