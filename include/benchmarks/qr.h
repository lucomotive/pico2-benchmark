#pragma once

#include "Eigen/Householder"
#include "timer.h"
#include "utils.h"

namespace benchmarks {
namespace qr {

template <typename Solver, typename Mat>
no_inline Time householder(Solver &qr, const Mat &source) {
  Timer timer;
  qr.compute(source);
  return timer.elapsed_us();
}

}; // namespace qr
}; // namespace benchmarks
