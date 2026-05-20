#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
  const Mat<P> source(Mat<P>::Random(size, size));
  Mat<P> res;
  auto time = inverse::inverse(res, source);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
                  "INVERSE:", res.format(EIGEN_FMT), "TIME:", time);
}
