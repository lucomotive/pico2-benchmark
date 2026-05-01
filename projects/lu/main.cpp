// set float or double
#include <cstdint>
#define REAL float

#include "pico/rand.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <iostream>

// Eigen
#include <Eigen/Dense>

#define Dyn Eigen::Dynamic
#define GenericMatrix Eigen::Matrix<REAL, Dyn, Dyn>

// nach diesem beispiel:
// https://libeigen.gitlab.io/eigen/docs-5.0/classEigen_1_1FullPivLU.html
template <bool debug>
std::tuple<uint64_t, double> inline run_benchmark(uint32_t x, uint32_t y) {
  GenericMatrix M(x, y);
  GenericMatrix L(GenericMatrix::Identity(x, x));
  // random values
  M.setRandom();

  // debug matrix M
  if (debug)
    std::cout << "M = " << std::endl << M << std::endl;

  // start clock
  absolute_time_t startTime = get_absolute_time();

  Eigen::FullPivLU<GenericMatrix> lu(M);
  L.block(0, 0, x, y).triangularView<Eigen::StrictlyLower>() = lu.matrixLU();
  GenericMatrix U(lu.matrixLU().triangularView<Eigen::Upper>());

  // stop clock
  absolute_time_t stopTime = get_absolute_time();

  // reverse engineer this
  GenericMatrix approxM(lu.permutationP().inverse() * L * U *
                        lu.permutationQ().inverse());

  double error = (M - approxM).squaredNorm();
  // debug results
  if (debug) {
    std::cout << "L = " << std::endl << L << std::endl;
    std::cout << "U = " << std::endl << U << std::endl;

    // reverse engineer this
    std::cout << "M ≈ " << std::endl << approxM << std::endl;
    std::cout << "Error = " << error << std::endl;
  }

  return {absolute_time_diff_us(startTime, stopTime), error};
}

int main() {
  stdio_init_all();

  // wait for PC to listen
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  std::cout << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
            << std::endl;
  run_benchmark<true>(5, 3);
  std::cout << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
            << std::endl;

  // print with csv format
  printf("n,X,Y,time_us,error\n");

  const uint32_t min_size = 5;
  const uint32_t max_size = 115; // max is 119x118 (out of memory error)

  int i = 0;
  for (int x = min_size; x < max_size; x++) {
    for (int y = min_size; y < max_size; y++) {
      if (x < y) // doesnt work if x < y
        continue;
      // flatten dataset by cutting out huge ratios
      // (who even needs a 100x5 matrix)
      if ((x < 0.3 * y) || (y < 0.3 * x))
        continue;

      auto [time_us, error] = run_benchmark<false>(x, y);
      printf("%u,%lu,%lu,%llu,%e\n", i, x, y, time_us, error);
      i++;
    }
  }

  fflush(stdout);
}
