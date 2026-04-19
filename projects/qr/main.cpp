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

  Eigen::Matrix<REAL, Dyn, Dyn> A = Eigen::Matrix<REAL, Dyn, Dyn>::Random();

  absolute_time_t startTime = get_absolute_time();
  for (uint16_t i = 0; i < batch_size; i++) {
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
  Eigen::Matrix<REAL, Dyn, Dyn> A = Eigen::Matrix<REAL, Dyn, Dyn>::Random();

  printf(A);

  fflush(stdout);
}
