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

uint32_t rand_range(uint32_t min, uint32_t max) {
  return min + (get_rand_32() % (max - min + 1));
}

uint64_t inline run_benchmark(uint32_t size, uint16_t batch_size) {
  // create array of matrices
  Eigen::Matrix<REAL, Dyn, Dyn> *matrices =
      new Eigen::Matrix<REAL, Dyn, Dyn>[batch_size];

  absolute_time_t startTime = get_absolute_time();
  for (uint16_t i = 0; i < batch_size; i++) {
    // create matrix and store in array (prevents optimization (hopefully))
    Eigen::Matrix<REAL, Dyn, Dyn> tmp(size, size);
    matrices[i] = tmp;
  }
  absolute_time_t stopTime = get_absolute_time();
  uint64_t deltaTime = absolute_time_diff_us(startTime, stopTime);

  delete[] matrices;
  return deltaTime / batch_size;
}

int main() {
  stdio_init_all();

  // wait for PC to listen
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  // print with csv format
  printf("n,size,time_us\n");

  const uint32_t max_matrix_size = 100;
  const uint32_t benchmark_batch_size = 10;
  for (uint16_t i = 0; i < max_matrix_size; i++) {
    uint16_t size = i;
    printf("%u,%lu,", i, size);

    uint64_t average_us = run_benchmark(size, benchmark_batch_size);
    printf("%llu\n", average_us);
  }
  fflush(stdout);
}
