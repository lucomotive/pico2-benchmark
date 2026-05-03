#pragma once
#include "../Benchmark.h"

#include "pico/rand.h"
#include "pico/time.h"
#include <iostream>

#include <nlohmann/json.hpp>

#include <Eigen/Dense>

template <typename P> class Inverse : public Benchmark {
private:
  uint32_t min_dim;
  uint32_t max_dim;

public:
  Inverse(uint32_t min_dim, uint32_t max_dim)
      : max_dim(max_dim), min_dim(min_dim) {}
  void run(bool debug) override;

  void sof() override;
};
template <typename P> void Inverse<P>::sof() {
  printf("SOF\n");
  printf("size,time_us\n");
}
template <typename P> void Inverse<P>::run(bool debug) {
#define Dyn Eigen::Dynamic
#define GenericMatrix Eigen::Matrix<P, Dyn, Dyn>

  uint32_t x = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);

  GenericMatrix M(x, x);
  // random values
  M.setRandom();

  // start clock
  absolute_time_t startTime = get_absolute_time();

  GenericMatrix R(M.inverse());

  // stop clock
  absolute_time_t stopTime = get_absolute_time();

  // prevents optimization
  volatile P sink = R(0, 0);
  (void)sink;

  uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
  if (!debug) {
    printf("%lu,%llu\n", x, time_us);
  }

  // debug results
  if (debug) {
    std::cout << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
              << std::endl;

    std::cout << "M = " << std::endl << M << std::endl;
    std::cout << "R = " << std::endl << R << std::endl;
    std::cout << "Time µs = " << time_us << std::endl;

    std::cout << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
              << std::endl;
  }
}
