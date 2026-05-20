#pragma once

#include "benchmarks/benchmarks.h"
#include "benchmarks/det.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, size_t SIZE> void debug() {
  const Matrix<P, SIZE, SIZE> source(Matrix<P, SIZE, SIZE>::Random());
  auto [time, res] = det::stack<P>(source);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "DETERMINANT:", res,
                  "TIME:", time);
}
