#pragma once

#include "benchmarks/benchmarks.h"
#include "print.h"
#include "timer.h"
#include <Eigen/Householder>

using namespace benchmarks;

template <typename P> void debug(uint32_t rows, uint32_t cols) {
  uint32_t size = rows;
  const Mat<P> source(Mat<P>::Random(rows, cols));
  Mat<P> temp;
  if (rows == cols)
    temp = source * source.adjoint();
  else
    temp = source.transpose() * source;

  LLT<Mat<P>> llt(size);
  auto time = llt::llt(llt, temp);

  print_all<'\n'>("SOURCE:", source.format(EIGEN_FMT),
                  "TEMP:", temp.format(EIGEN_FMT),
                  "LLT:", llt.matrixLLT().format(EIGEN_FMT), "TIME:", time);
}
