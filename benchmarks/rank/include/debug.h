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
  source.setRandom();

  FullPivLU<Mat<P>> lu(source);
  auto [time, rank] = rank::rank(lu);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "RANK:", rank,
                  "TIME:", time);
}
