#pragma once

#include "timer.h"
#include "utils.h"

namespace benchmarks {
namespace llt {
using namespace my_timer;

template <typename Solver, typename Mat>
no_inline Time llt(Solver &solver, const Mat &A) {
  Timer timer;
  solver.compute(A);
  return timer.elapsed_us();
}

}; // namespace llt
}; // namespace benchmarks
