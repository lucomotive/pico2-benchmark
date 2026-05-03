#pragma once
#include "../Benchmark.h"

#include "pico/rand.h"
#include "pico/time.h"
#include <iostream>

#include <nlohmann/json.hpp>

#include <Eigen/Dense>

template <typename P> class MatMul : public Benchmark {
private:
  uint32_t min_dim;
  uint32_t max_dim;

public:
  MatMul(uint32_t min_dim, uint32_t max_dim)
      : max_dim(max_dim), min_dim(min_dim) {}
  void run(bool debug) override;

  void sof() override;
};

template <typename P> void MatMul<P>::sof() {
  printf("SOF\n");
  printf("X,Y,Z,time_us\n");
}

template <typename P> void MatMul<P>::run(bool debug) {
#define Dyn Eigen::Dynamic
#define GenericMatrix Eigen::Matrix<P, Dyn, Dyn>

  uint32_t x = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);
  uint32_t y = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);
  uint32_t z = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);

  // allocate matrices
  GenericMatrix M1(x, y);
  GenericMatrix M2(y, z);

  // write random values
  M1.setRandom();
  M2.setRandom();

  // start clock
  absolute_time_t startTime = get_absolute_time();
  // perform calculation
  GenericMatrix R(M1 * M2);
  // stop clock
  absolute_time_t stopTime = get_absolute_time();

  // prevent optimization (hopefully)
  volatile P sink = R(0, 0);
  (void)sink;

  uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
  if (!debug) {
    printf("%lu,%lu,%lu,%llu\n", x, y, z, time_us);
  }

  // debug results
  if (debug) {
    std::cout << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
              << std::endl;

    std::cout << "M1 = " << std::endl << M1 << std::endl;
    std::cout << "M2 = " << std::endl << M2 << std::endl;
    std::cout << "R = " << std::endl << R << std::endl;

    std::cout << "Time µs = " << time_us << std::endl;

    std::cout << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
              << std::endl;
  }
}
