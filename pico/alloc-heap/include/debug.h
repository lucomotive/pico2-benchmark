#pragma once

#include "benchmarks.h"
#include "benchmarks/det.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
  Mat<P> res;
  auto time = alloc::heap(res, size);

  print_all<'\n'>("SOURCE:", res.format(EIGEN_FMT), "TIME:", time);
}
