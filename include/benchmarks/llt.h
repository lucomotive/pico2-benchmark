#pragma once

#include "timer.h"
#include "utils.h"

namespace benchmarks {
namespace llt {

template <typename S> no_inline Time llt(LLT<Mat<S>> &solver, const Mat<S> &A) {
  Timer timer;
  solver.compute(A);
  return timer.elapsed();
}

}; // namespace llt
}; // namespace benchmarks
