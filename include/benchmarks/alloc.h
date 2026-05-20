#pragma once

#include "const_rand.h"
#include "timer.h"
#include "utils.h"
#include <Eigen/Dense>

namespace benchmarks {
namespace alloc {

template <typename S> no_inline Time heap(Mat<S> &res, uint32_t size) {
  Timer timer;
  res = Mat<S>(size);
  return timer.elapsed();
}

template <typename S, uint32_t SIZE>
no_inline Time stack(Matrix<S, SIZE, SIZE> &res) {
  Timer timer;
  res = Matrix<S, SIZE, SIZE>();
  return timer.elapsed();
}

// initialize data in flash
/* check if compiles to flash:
  arm-none-eabi-nm --size-sort --print-size \
  pico/build/read-flash/read-flash-float.elf | c++filt |  grep data\<
 */
const uint16_t FLASH_DATA_SIZE = 100; // length doesnt matter
// if matrix is bigger, it just reads other flash data. does not impact the
// benchmark
template <typename P>
const std::array<P, FLASH_DATA_SIZE> read_flash_data =
    const_rand::random_float_array<P, FLASH_DATA_SIZE>();

template <typename S>
no_inline Time from_flash(Mat<S> &res, uint32_t rows, uint32_t cols) {
  Timer timer;
  res = Eigen::Map<const Mat<S>>(read_flash_data<S>.data(), rows, cols);
  return timer.elapsed();
}

}; // namespace alloc
}; // namespace benchmarks
