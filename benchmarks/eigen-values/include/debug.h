#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <uint32_t SIZE, typename P> void debug(uint32_t rows, uint32_t cols) {
  EigenSolver<Mat<P>> solver(rows, cols);
  Mat<P> source(Mat<P>::Random(rows, cols));
  auto time = eigen::eigen(solver, source);

  bool success = solver.info() == Success;
  auto values = solver.eigenvalues();
  auto vectors = solver.eigenvectors();

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "COMPUTABLE:", success,
                  "VALUES:", values.format(EIGEN_FMT),
                  "VECTORS:", vectors.format(EIGEN_FMT), "TIME:", time);
}
