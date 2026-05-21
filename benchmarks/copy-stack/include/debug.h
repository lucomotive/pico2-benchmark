#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P, uint32_t SIZE> void debug() {
  const Matrix<P, SIZE, SIZE> source(Matrix<P, SIZE, SIZE>::Random(SIZE, SIZE));
  Matrix<P, SIZE, SIZE> res;
  auto time = copy::stack<P, SIZE>(res, source);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
                  "COPY:", res.format(EIGEN_FMT), "TIME:", time);
}
