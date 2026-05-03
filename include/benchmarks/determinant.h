#pragma once
#include "../Benchmark.h"

#include "pico/rand.h"
#include "pico/time.h"
#include <iostream>

#include <nlohmann/json.hpp>

#include <Eigen/Dense>

template <typename P> class Determinant : public Benchmark {
private:
  uint32_t min_dim;
  uint32_t max_dim;

public:
  Determinant(uint32_t min_dim, uint32_t max_dim)
      : max_dim(max_dim), min_dim(min_dim) {}
  void run(bool debug) override;

  void sof() override;
};
template <typename P> void Determinant<P>::sof() {
  printf("SOF\n");
  printf("size,time_us\n");
}
template <typename P> void Determinant<P>::run(bool debug) {
#define Dyn Eigen::Dynamic
#define GenericMatrix Eigen::Matrix<P, Dyn, Dyn>

  uint32_t x = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);
  // uint32_t y = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);

  GenericMatrix A(x, x);
  // random values
  A.setRandom();

  // start clock
  absolute_time_t startTime = get_absolute_time();

  P res = A.determinant();

  // stop clock
  absolute_time_t stopTime = get_absolute_time();

  // prevent optimization (hopefully)
  (void)res;

  uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
  if (!debug) {
    printf("%lu,%llu\n", x, time_us);
  }

  // debug results
  if (debug) {
    std::cout << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
              << std::endl;

    std::cout << "A = " << std::endl << A << std::endl;
    std::cout << "R = " << std::endl << res << std::endl;

    std::cout << "Time µs = " << time_us << std::endl;

    std::cout << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
              << std::endl;
  }
}
