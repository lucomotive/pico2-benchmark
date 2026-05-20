#pragma once

#include "benchmarks.h"
#include "print.h"
#include <Eigen/src/QR/HouseholderQR.h>

using namespace benchmarks;

template <typename P> void debug(uint32_t rows, uint32_t cols) {
  Mat<P> source(Mat<P>::Random(rows, cols));
  HouseholderQR<Mat<P>> qr(rows, cols);
  auto time = qr::householder(qr, source);

  // reconstruct source
  Mat<P> Q(qr.householderQ());
  Mat<P> R(qr.matrixQR().template triangularView<Upper>());

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
                  "QR:", qr.matrixQR().format(EIGEN_FMT),
                  "Q:", Q.format(EIGEN_FMT), "R:", R.format(EIGEN_FMT),
                  "Q * R:", (Q * R).format(EIGEN_FMT),
                  "ERROR:", (source - (Q * R)).norm(), "TIME:", time);
}
