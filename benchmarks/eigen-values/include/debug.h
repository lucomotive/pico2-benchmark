#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Dense>

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
  EigenSolver<Mat<P>> solver;
  Mat<P> source(Mat<P>::Random(size, size));
  auto time = eigen::eigen(solver, source);

  bool success = solver.info() == Success;
  auto values = solver.eigenvalues();
  auto vectors = solver.eigenvectors();

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "COMPUTABLE:", success,
                  "VALUES:", values.format(EIGEN_FMT),
                  "VECTORS:", vectors.format(EIGEN_FMT), "TIME:", time);
}
