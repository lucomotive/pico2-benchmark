#pragma once

#include "timer.h"
#include "utils.h"
#include <Eigen/Householder>

namespace benchmarks {
namespace lu {
using namespace my_timer;

template <typename Solver, typename Mat>
no_inline Time full_piv(Solver &lu, const Mat &source) {
  Timer timer;
  lu.compute(source);
  return timer.elapsed_us();
}

}; // namespace lu
}; // namespace benchmarks
