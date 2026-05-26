#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
#if defined(ENABLE_PSRAM)
  auto *s_cache = (uint8_t *)PSRAM_BASE;
  Map<Mat<P>> source((P *)s_cache, size, size);
#else
  Mat<P> source(size, size);
#endif
  source.setRandom();

  EigenSolver<Mat<P>> solver;
  auto time = eigen::eigen(solver, source);
  bool success = solver.info() == Success;

  const auto values = solver.eigenvalues();
  const auto vectors = solver.eigenvectors();

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "COMPUTABLE:", success,
                  "VALUES:", values.format(EIGEN_FMT),
                  "VECTORS:", vectors.format(EIGEN_FMT), "TIME:", time);
}
