#pragma once

#include "const_rand.h"
#include "my_rand.h"
#include "timer.h"
#include "utils.h"
#include <Eigen/Dense>

namespace benchmarks {
namespace alloc {
using namespace my_timer;

template <typename S> no_inline std::tuple<Mat<S>, Time> heap(uint16_t size) {
  Timer timer;
  Mat<S> res(size, size);
  auto time = timer.elapsed_us();
  res.setRandom();
  return {std::move(res), time};
}

template <typename S, uint32_t SIZE>
no_inline std::tuple<Matrix<S, SIZE, SIZE>, Time> stack() {
  Timer timer;
  Matrix<S, SIZE, SIZE> res;
  auto time = timer.elapsed_us();
  res.setRandom();
  return {std::move(res), time};
}

#if defined(BUILD_PICO)
// initialize data in flash
/* check if compiles to flash:
  arm-none-eabi-nm --size-sort --print-size \
  pico/build/read-flash/read-flash-float.elf | c++filt |  grep data\<
 */

constexpr uint16_t MAX_FLASH_DATA_SIZE = 100 * 100;
// flash data with fixed size, to keep flash size from blowing up. on pico size
// doesnt matter. if matrix is bigger, it just reads other flash data. does not
// impact the benchmark
template <typename P>
const std::array<P, MAX_FLASH_DATA_SIZE> read_flash_data =
    const_rand::random_float_array<P, MAX_FLASH_DATA_SIZE>();

template <typename S>
no_inline std::tuple<Mat<S>, Time> from_flash_heap(uint16_t size) {
  Timer timer;
  Mat<S> res = Eigen::Map<const Mat<S>>(read_flash_data<S>.data(), size, size);
  auto time = timer.elapsed_us();
  return {std::move(res), time};
}

template <typename S, uint16_t SIZE>
no_inline std::tuple<Matrix<S, SIZE, SIZE>, Time> from_flash_stack() {
  Timer timer;
  Matrix<S, SIZE, SIZE> res = Eigen::Map<const Matrix<S, SIZE, SIZE>>(
      read_flash_data<S>.data(), SIZE, SIZE);
  auto time = timer.elapsed_us();
  return {std::move(res), time};
}

#endif

}; // namespace alloc
}; // namespace benchmarks
