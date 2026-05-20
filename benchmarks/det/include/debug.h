#pragma once

#include "benchmarks/benchmarks.h"
#include "benchmarks/det.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
  const Mat<P> source(Mat<P>::Random(size, size));
  auto [time, res] = det::determinant(source);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "DETERMINANT:", res,
                  "TIME:", time);
}
