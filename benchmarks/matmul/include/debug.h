#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Dense>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> void debug(uint32_t x, uint32_t y, uint32_t z) {
#if defined(ENABLE_PSRAM)
  auto *m1_cache = (uint8_t *)PSRAM_BASE;
  auto *m2_cache = m1_cache + (sizeof(P) * x * y);
  auto *r_cache = m2_cache + (sizeof(P) * x * z);
  Map<Mat<P>> M1((P *)m1_cache, x, y);
  Map<Mat<P>> M2((P *)m2_cache, y, z);
  Map<Mat<P>> R((P *)r_cache, x, z);
#else
  Mat<P> M1(x, y);
  Mat<P> M2(y, z);
  Mat<P> R;
#endif

  M1.setRandom();
  M2.setRandom();

  const auto time = matmul::matmul(M1, M2, R);

  printf("M1:\n");
  print_float_matrix(M1);

  printf("M2:\n");
  print_float_matrix(M2);

  printf("RES:\n");
  print_float_matrix(R);

  printf("TIME: %llu\n", time);
  // print_all<'\n'>("M1:", M1.format(EIGEN_FMT), "M2:", M2.format(EIGEN_FMT),
  //                 "RES:", R.format(EIGEN_FMT), "TIME:", time);
}
