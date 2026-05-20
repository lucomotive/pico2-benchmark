#pragma once

#include "timer.h"
#include "utils.h"

namespace benchmarks {
namespace qr {

template <typename S>
no_inline Time householder(HouseholderQR<Mat<S>> &qr, const Mat<S> &source) {
  Timer timer;
  qr.compute(source);
  return timer.elapsed();
}

}; // namespace qr
}; // namespace benchmarks
