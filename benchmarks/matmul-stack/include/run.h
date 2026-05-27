#pragma once

#include "benchmarks/benchmarks.h"
#include "repeat.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P, uint32_t x, uint32_t y, uint32_t z> inline void op() {
  Matrix<P, x, y> M1;
  Matrix<P, y, z> M2;
  Matrix<P, x, z> RES;

  M1.setRandom();
  M2.setRandom();

  auto time = matmul::matmul(M1, M2, RES);
  printf("%u,%u,%u,%lu\n", x, y, z, time);
};

template <typename P> void run() {
  // run benchmark
  printf("x,y,z,time_us\n");
  constexpr uint16_t min = 5;
  constexpr uint16_t max = 100;
  constexpr uint16_t step = 2;

  // compile time is too long. no nesting!
  repeat<max, step, min>([](const auto i) { op<P, i, i, i>(); });
}
