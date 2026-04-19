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

uint64_t inline run_benchmark(uint32_t x, uint32_t y, uint16_t batch_size) {

  Eigen::Matrix<REAL, Dyn, Dyn> A(x, y);
  A.setRandom();
  Eigen::HouseholderQR<Eigen::Matrix<REAL, Dyn, Dyn>> qr(A);

  Eigen::Matrix<REAL, Dyn, Dyn> thinQ =
      Eigen::Matrix<REAL, Dyn, Dyn>::Identity(x, y);

  Eigen::Matrix<REAL, Dyn, Dyn> Q;
  Eigen::Matrix<REAL, Dyn, Dyn> R;

  absolute_time_t startTime = get_absolute_time();
  for (uint16_t i = 0; i < batch_size; i++) {
    Q.noalias() = qr.householderQ() * thinQ;
    R.noalias() = Q.transpose() * A;
  }
  absolute_time_t stopTime = get_absolute_time();
  uint64_t deltaTime = absolute_time_diff_us(startTime, stopTime);

  return deltaTime / batch_size;
}

int main() {
  stdio_init_all();

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
    uint32_t y = rand_range(min_size, x);

    printf("%u,%lu,%lu,", i, x, y);

    uint64_t average_us = run_benchmark(x, y, 30);
    printf("%llu\n", average_us);
  }

  fflush(stdout);
}
