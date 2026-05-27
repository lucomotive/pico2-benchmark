#pragma once

#include "benchmarks/benchmarks.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> inline void op(uint16_t size) {
  auto [res, time] = alloc::from_flash_heap<P>(size);
  printf("%u,%llu\n", size, time);
};

template <typename P> void loop(uint16_t min, uint16_t max, uint16_t step) {
  for (uint16_t i = min; i < max; i += step)
    op<P>(i);
}

template <typename P> void run() {
  // run benchmark
  printf("rows/cols,time_us\n");
  loop<P>(5, 50, 2);
  loop<P>(50, 100, 3);
  loop<P>(100, 500, 5);
}
