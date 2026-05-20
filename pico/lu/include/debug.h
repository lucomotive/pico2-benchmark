#pragma once

#include "benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t rows, uint32_t cols) {
  Mat<P> source(Mat<P>::Random(rows, cols));
  FullPivLU<Mat<P>> lu(rows, cols);
  auto time = lu::full_piv(lu, source);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
                  "LU:", lu.matrixLU().format(EIGEN_FMT), "TIME:", time);
}
