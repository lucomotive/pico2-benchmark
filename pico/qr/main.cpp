#include "pico/stdio_usb.h"
#include "pico/time.h"

#include <Eigen/QR>
#include <iostream>
#include <tuple>

#define no_inline __attribute__((noinline))

using namespace Eigen;
template <typename S> using Mat = Matrix<S, Dynamic, Dynamic>;

template <typename QR>
no_inline void compute(QR &qr, const typename QR::MatrixType &A) {
  qr.compute(A);
}

template <typename S> struct result_t {
  Mat<S> source;
  Mat<S> result;
};

template <typename S> result_t<S> householder_qr(uint32_t rows, uint32_t cols) {
  auto source = Mat<S>::Random(rows, cols);
  HouseholderQR<Mat<S>> qr(rows, cols);

  compute(qr, source);

  auto result = qr.matrixQR();
  return {source, result};
}

int main() {
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  // run benchmark
  auto [s, r] = householder_qr<float>(5, 5);
  std::cout << "SOURCE = " << std::endl
            << s << std::endl
            << "RESULT = " << std::endl
            << r;
}
