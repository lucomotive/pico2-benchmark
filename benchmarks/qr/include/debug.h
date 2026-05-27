#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include <Eigen/Householder>

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

using namespace benchmarks;

template <typename P> void debug(uint32_t rows, uint32_t cols) {
#if defined(ENABLE_PSRAM)
  auto *cache = (uint8_t *)PSRAM_BASE;
  Map<Mat<P>> source((P *)cache, rows, cols);
#else
  Mat<P> source(rows, cols);
#endif
  source.setRandom();

  HouseholderQR<Mat<P>> qr(rows, cols);
  auto time = qr::householder(qr, source);

  // reconstruct source
  Mat<P> Q(qr.householderQ());
  Mat<P> R(qr.matrixQR().template triangularView<Upper>());

  printf("SOURCE:\n");
  print_float_matrix(source);
  printf("QR:\n");
  print_float_matrix(qr.matrixQR());
  printf("Q:\n");
  print_float_matrix(Q);
  printf("R:\n");
  print_float_matrix(R);
  printf("Q*R:\n");
  print_float_matrix(Q * R);
  printf("ERROR: %.17g\n", (source - (Q * R)).norm());
  printf("TIME: %llu\n", time);

  // const uint32_t repeat = 50;
  // std::cout << std::string(repeat, '-') << std::endl;
  // print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
  //                 "QR:", qr.matrixQR().format(EIGEN_FMT),
  //                 "Q:", Q.format(EIGEN_FMT), "R:", R.format(EIGEN_FMT),
  //                 "Q * R:", (Q * R).format(EIGEN_FMT),
  //                 "ERROR:", (source - (Q * R)).norm(), "TIME:", time);
  // std::cout << std::string(repeat, '-') << std::endl;
}
