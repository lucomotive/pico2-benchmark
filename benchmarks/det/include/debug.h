#pragma once

#include "benchmarks/benchmarks.h"
#include "benchmarks/det.h"
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

  auto [time, res] = det::determinant<P>(source);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT), "DETERMINANT:", res,
                  "TIME:", time);
}
