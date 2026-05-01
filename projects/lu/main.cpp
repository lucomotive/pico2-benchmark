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

uint64_t inline run_benchmark(uint32_t x, uint32_t y) {
  GenericMatrix M(x, y);
  GenericMatrix L(GenericMatrix::Identity(x, x));
  // random values
  M.setRandom();

  // start clock
  absolute_time_t startTime = get_absolute_time();

  Eigen::FullPivLU<GenericMatrix> lu(M);
  L.block(0, 0, x, y).triangularView<Eigen::StrictlyLower>() = lu.matrixLU();
  GenericMatrix U(lu.matrixLU().triangularView<Eigen::Upper>());

  // stop clock
  absolute_time_t stopTime = get_absolute_time();

  return absolute_time_diff_us(startTime, stopTime);
}

int main() {
  stdio_init_all();

  // wait for PC to listen
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  // print with csv format
  printf("n,X,Y,time_us\n");

  const uint32_t min_size = 2;
  const uint32_t max_size = 100;

  int i = 0;
  for (int x = min_size; x < max_size; x++) {
    for (int y = min_size; y < max_size; y++) {
      printf("%u,%lu,%lu,", i, x, y);
      uint64_t time_us = run_benchmark(x, y);
      printf("%llu\n", time_us);
      i++;
    }
  }

  fflush(stdout);
}
