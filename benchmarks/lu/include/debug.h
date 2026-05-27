#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> void debug(uint32_t rows, uint32_t cols) {
#if defined(ENABLE_PSRAM)
  auto *s_cache = (uint8_t *)PSRAM_BASE;
  Map<Mat<P>> source((P *)s_cache, rows, cols);
#else
  Mat<P> source(rows, cols);
#endif

  FullPivLU<Mat<P>> lu(rows, cols);
  const auto time = lu::full_piv(lu, source);

  printf("SOURCE:\n");
  print_float_matrix(source);
  printf("LU:\n");
  print_float_matrix(lu.matrixLU());
  printf("TIME: %llu\n", time);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
                  "LU:", lu.matrixLU().format(EIGEN_FMT), "TIME:", time);
}
