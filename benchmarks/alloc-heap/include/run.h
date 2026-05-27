#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <cstdint>
#include <cstdio>

using namespace benchmarks;

template <typename P> void loop(uint16_t min, uint16_t max, uint16_t step) {
  for (uint16_t size = min; size <= max; size += (int)step) {
    auto [mat, time] = alloc::heap<P>(size);
    printf("%u,%u,%llu\n", size, size, time);
  }
}

template <typename P> void run() {
  printf("rows,cols,time_us\n");
  loop<P>(5, 500, 2);
}
