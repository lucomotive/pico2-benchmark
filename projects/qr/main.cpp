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

uint32_t rand_range(uint32_t min, uint32_t max) {
  return min + (get_rand_32() % (max - min + 1));
}

template <bool debug>
std::tuple<uint64_t, double> inline run_benchmark(uint32_t x, uint32_t y) {
  GenericMatrix A(x, y);
  // random values
  A.setRandom();

  // debug matrix M
  if (debug)
    std::cout << "A = " << std::endl << A << std::endl;

  // start clock
  absolute_time_t startTime = get_absolute_time();

  Eigen::HouseholderQR<GenericMatrix> qr(A);
  GenericMatrix thinQ(GenericMatrix::Identity(x, y));

  GenericMatrix Q(qr.householderQ() * thinQ);
  GenericMatrix R(Q.transpose() * A);

  // stop clock
  absolute_time_t stopTime = get_absolute_time();

  // reverse engineer this
  GenericMatrix approxA(Q * R);
  double error = (A - approxA).squaredNorm();

  // debug results
  if (debug) {
    std::cout << "Q = " << std::endl << Q << std::endl;
    std::cout << "R = " << std::endl << R << std::endl;

    // reverse engineer this
    std::cout << "A ≈ " << std::endl << approxA << std::endl;
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
  const uint32_t max_size = 125; // max is 126x125 (out of memory error)

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
