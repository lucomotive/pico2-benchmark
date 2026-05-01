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



template <bool debug> uint64_t inline run_benchmark(uint32_t size) {
  // allocate matrices on stack
  GenericMatrix M(size, size);
  // write random values
  M.setRandom();

  // debug matrix M
  if (debug)
    std::cout << "M = " << std::endl << M << std::endl;

  absolute_time_t startTime = get_absolute_time();
  GenericMatrix R(M.inverse());
  absolute_time_t stopTime = get_absolute_time();

  // debug results
  if (debug)
    std::cout << "R = " << std::endl << R << std::endl;

  return absolute_time_diff_us(startTime, stopTime);
}

int main() {
  stdio_init_all();

  // wait for PC to listen
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  std::cout << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
            << std::endl;
  run_benchmark<true>(5);
  std::cout << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
            << std::endl;

  // print with csv format
  printf("size,time_us\n");

  const uint32_t min_size = 5;
  const uint32_t max_size = 140; // crashes after that
  for (uint32_t i = min_size; i < max_size; i++) {
    uint64_t time_us = run_benchmark<false>(i);
    printf("%lu,%llu\n", i, time_us);
  }
  fflush(stdout);
}
