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
  auto *r_cache = s_cache + (sizeof(P) * size * size);
  Map<Mat<P>> source((P *)s_cache, size, size);
  Map<Mat<P>> res((P *)r_cache, size, size);
#else
  Mat<P> source(size, size);
  Mat<P> res;
#endif
  source.setRandom();
  res.setRandom();

  const auto time = copy::copy(res, source);

  printf("SOURCE:\n");
  print_float_matrix(source);
  printf("COPY:\n");
  print_float_matrix(res);
  printf("TIME: %llu\n", time);
  // print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
  //                 "COPY:", res.format(EIGEN_FMT), "TIME:", time);
}
