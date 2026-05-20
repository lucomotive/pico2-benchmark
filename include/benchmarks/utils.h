#pragma once

#include "Eigen/Dense"

namespace benchmarks {
#define no_inline __attribute__((noinline))
using namespace Eigen;
template <typename S> using Mat = Matrix<S, Dynamic, Dynamic>;
}; // namespace benchmarks
