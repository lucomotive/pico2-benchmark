#pragma once

#include "const_rand.h"
#include "timer.h"
#include "utils.h"
#include <Eigen/Dense>

namespace benchmarks {
namespace copy {

template <typename S> no_inline Time heap(Mat<S> &res, const Mat<S> &source) {
  Timer timer;
  res = source;
  return timer.elapsed_us();
}

template <typename S, uint32_t SIZE>
no_inline Time stack(Matrix<S, SIZE, SIZE> &res,
                     const Matrix<S, SIZE, SIZE> &source) {
  Timer timer;
  res = source;
  return timer.elapsed_us();
}

#if defined(BUILD_PICO)
// initialize data in flash
/* check if compiles to flash:
  arm-none-eabi-nm --size-sort --print-size \
  pico/build/read-flash/read-flash-float.elf | c++filt |  grep data\<
 */

// on pico size doesnt matter. if matrix is bigger, it just reads other flash
// data. does not impact the benchmark
template <typename P, uint16_t SIZE>
const std::array<P, 100> read_flash_data =
    const_rand::random_float_array<P, 100>();
#elif defined(BUILD_HOST)
template <typename P, uint16_t SIZE>
const std::array<P, SIZE> read_flash_data =
    const_rand::random_float_array<P, SIZE>();
#endif

template <typename S, uint16_t SIZE> no_inline Time from_flash(Mat<S> &res) {
  Timer timer;
  res = Eigen::Map<const Mat<S>>(read_flash_data<S, SIZE * SIZE>.data(), SIZE,
                                 SIZE);
  return timer.elapsed_us();
}

}; // namespace copy
}; // namespace benchmarks
