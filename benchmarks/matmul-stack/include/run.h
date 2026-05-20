#pragma once

#include "benchmarks/benchmarks.h"
#include "utils.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P, uint32_t x, uint32_t y, uint32_t z> inline void op() {
  Matrix<P, x, y> M1(Matrix<P, x, y>::Random());
  Matrix<P, y, z> M2(Matrix<P, y, z>::Random());
  Matrix<P, x, z> RES;
  auto time = matmul::stack<P>(M1, M2, RES);
  printf("%u,%u,%u,%llu\n", y, x, z, time);
};

template <typename P> void run() {
  // run benchmark
  printf("x,y,z,time_us\n");
  constexpr uint16_t min = 5;
  constexpr uint16_t max = 100;
  constexpr uint16_t step = 2;

  repeat<max, step, min>([&](auto x) { op<P, x, x, x>(); });
}
