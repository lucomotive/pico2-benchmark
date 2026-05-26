#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Householder>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> void debug(uint32_t size) {
#if defined(ENABLE_PSRAM)
  auto *s_cache = (uint8_t *)PSRAM_BASE;
  auto *t_cache = s_cache + (sizeof(P) * size * size);
  Map<Mat<P>> source((P *)s_cache, size, size);
  Map<Mat<P>> temp((P *)t_cache, size, size);
#else
  Mat<P> source(rows, cols);
  Mat<P> temp;
#endif
  source.setRandom();

  temp = source * source.adjoint();

  LLT<Mat<P>> llt(size);
  auto time = llt::llt(llt, temp);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
                  "TEMP:", temp.format(EIGEN_FMT),
                  "LLT:", llt.matrixLLT().format(EIGEN_FMT), "TIME:", time);
}
