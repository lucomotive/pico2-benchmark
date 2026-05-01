// set float or double
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
uint64_t inline run_benchmark(uint32_t x, uint32_t y, uint32_t z) {
  // allocate matrices
  GenericMatrix M1(x, y);
  GenericMatrix M2(y, z);
  // GenericMatrix R(x, z);

  // write random values
  M1.setRandom();
  M2.setRandom();

  // print matrices
  if (debug) {
    std::cout << "M1 = " << std::endl << M1 << std::endl;
    std::cout << "M2 = " << std::endl << M2 << std::endl;
  }

  absolute_time_t startTime = get_absolute_time();
  GenericMatrix R(M1 * M2);
  absolute_time_t stopTime = get_absolute_time();
  uint64_t deltaTime = absolute_time_diff_us(startTime, stopTime);

  if (debug)
    std::cout << "R = " << std::endl << R << std::endl;

  return deltaTime;
}

int main() {

  stdio_init_all();
  // sleep_ms(2000);

  // wait for PC to listen
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  std::cout << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
            << std::endl;
  run_benchmark<true>(3, 3, 3);
  std::cout << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
            << std::endl;

  // print with csv format
  printf("n,X,Y,Z,time_us\n");

  const uint32_t min_size = 10;
  const uint32_t max_size = 150;
  const uint32_t iterations = 10000;
  for (int i = 0; i < iterations; i++) {
    uint32_t x = rand_range(min_size, max_size);
    uint32_t y = rand_range(min_size, max_size);
    uint32_t z = rand_range(min_size, max_size);

    uint64_t time_us = run_benchmark<false>(x, y, z);
    printf("%u,%lu,%lu,%lu,%llu\n", i, x, y, z, time_us);
  }
  fflush(stdout);
}
