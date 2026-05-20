#include "pico/stdio_usb.h"
#include "pico/time.h"

#include <Eigen/QR>
#include <Eigen/src/Core/util/Constants.h>
#include <cstdio>
#include <iostream>
#include <pico/types.h>

#define no_inline __attribute__((noinline))

using namespace Eigen;
template <typename S> using Mat = Matrix<S, Dynamic, Dynamic>;

template <typename QR>
no_inline uint64_t compute(QR &qr, const typename QR::MatrixType &A) {
  auto start = get_absolute_time();
  qr.compute(A);
  auto stop = get_absolute_time();
  return absolute_time_diff_us(start, stop);
}

template <typename S> struct result_t {
  Mat<S> source;
  Mat<S> Q;
  Mat<S> R;
  uint64_t time;
};

template <typename S>
inline result_t<S> householder_qr(uint32_t rows, uint32_t cols) {
  Mat<S> source(Mat<S>::Random(rows, cols));
  HouseholderQR<Mat<S>> qr(rows, cols);

  auto time = compute(qr, source);

  Mat<S> Q(qr.householderQ());
  Mat<S> R(qr.matrixQR().template triangularView<Upper>());

  return {source, Q, R, time};
}

template <typename S> void print(const result_t<S> &result, bool verbose) {
  if (verbose) {
    std::cout << "––––––––––––––––– TESTING ALGORITHM –––––––––––––––––"
              << std::endl;
    std::cout << "SOURCE = " << std::endl << result.source << std::endl;
    std::cout << "Q = " << std::endl << result.Q << std::endl;
    std::cout << "R = " << std::endl << result.R << std::endl;
    auto approx = result.Q * result.R;
    std::cout << "Q * R = " << std::endl << result.Q * result.R << std::endl;
    std::cout << "Error = " << (result.source - approx).norm() << std::endl;
    std::cout << "TIME = " << result.time << "µs" << std::endl;
    std::cout << "–––––––––––––––––––––––––––––––––––––––––––––––––––––"
              << std::endl;
  } else {
    auto approx = result.Q * result.R;
    printf("%u,%u,%llu,%e\n", result.source.rows(), result.source.cols(),
           result.time, (result.source - approx).norm());
  }
}

int main() {
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  // display
  auto result = householder_qr<float>(3, 3);
  print(result, true);

  // run benchmark
  printf("x,y,time_us,error\n");
  const uint16_t min = 4;
  const uint16_t max = 100;
  const uint16_t step = 3;
  const float max_ratio = 0.3;
  for (uint16_t x = min; x <= max; x += (int)step) {
    for (uint16_t y = min; y <= x; y += (int)step) {
      if ((float)y < (float)x * max_ratio)
        continue;
      auto result = householder_qr<float>(x, y);
      print(result, false);
      if (x != y) {
        auto result = householder_qr<float>(y, x);
        print(result, false);
      }
    }
  }
}
