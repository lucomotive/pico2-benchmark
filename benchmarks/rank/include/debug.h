#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t rows, uint32_t cols) {
  Mat<P> source(Mat<P>::Random(rows, cols));
  FullPivLU<Mat<P>> lu(source);
  auto [time, rank] = rank::rank(lu);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "RANK:", rank,
                  "TIME:", time);
}
