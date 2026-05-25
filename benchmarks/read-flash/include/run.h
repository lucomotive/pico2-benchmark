#pragma once

#include "benchmarks/benchmarks.h"
#include "repeat.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint16_t size) {
  auto [res, time] = alloc::from_flash_heap<P>(size);
  printf("%u,%llu\n", size, time);
};

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  const uint16_t min = 5;
  const uint16_t max = 100;
  const uint16_t step = 2;

  for (uint16_t i = min; i < max; i += step)
    op<P>(i);
}
