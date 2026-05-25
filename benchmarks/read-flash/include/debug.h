#pragma once

#include "benchmarks/alloc.h"
#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint16_t size) {
  auto [res, time] = alloc::from_flash_heap<P>(size);

  print_all<'\n'>("RESULT:", res.format(EIGEN_FMT), "TIME:", time);
}
