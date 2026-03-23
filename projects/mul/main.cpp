// set float or double
#define REAL float

#include "pico/rand.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <iostream>

// Eigen
#include <Eigen/Dense>

#define Dyn Eigen::Dynamic

uint32_t rand_range(uint32_t min, uint32_t max) {
  return min + (get_rand_32() % (max - min + 1));
}

uint64_t inline run_benchmark(uint32_t x, uint32_t y, uint32_t z,
                              uint16_t iterations) {

  // allocate matrices on stack
  Eigen::Matrix<REAL, Dyn, Dyn> mat1(x, y);
  Eigen::Matrix<REAL, Dyn, Dyn> mat2(y, z);

  Eigen::Matrix<REAL, Dyn, Dyn> res(x, z);

  // write random values
  mat1.setRandom();
  mat2.setRandom();

  absolute_time_t startTime = get_absolute_time();
  // run benchmark x times
  for (uint16_t i = 0; i < iterations; i++) {
    res.noalias() = mat1 * mat2;
  }
  absolute_time_t stopTime = get_absolute_time();
  uint64_t deltaTime = absolute_time_diff_us(startTime, stopTime);

  return deltaTime / iterations;
}

int main() {

  stdio_init_all();
  // sleep_ms(2000);

  // wait for PC to listen
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  // print with csv format
  printf("n,X,Y,time_us\n");

  const uint32_t benchmark_count = 200;
  const uint32_t min_size = 5;
  const uint32_t max_size = 100;
  for (int i = 0; i < benchmark_count; i++) {
    // generate random size
    // max at 100x100 because anything higher leads to an out of memory panic
    uint32_t x = rand_range(min_size, max_size);
    uint32_t y = rand_range(min_size, max_size);
    uint32_t z = rand_range(min_size, max_size);

    printf("%u,%lu,%lu,%lu,", i, x, y, z);

    uint64_t average_us = run_benchmark(x, y, z, 20);
    printf("%llu\n", average_us);
  }
  fflush(stdout);
}
