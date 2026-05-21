#pragma once

#include "Eigen/Householder"
#include "timer.h"
#include "utils.h"

namespace benchmarks {
namespace qr {

template <typename S>
no_inline Time householder(HouseholderQR<Mat<S>> &qr, const Mat<S> &source) {
  Timer timer;
  qr.compute(source);
  return timer.elapsed_us();
}

}; // namespace qr
}; // namespace benchmarks
