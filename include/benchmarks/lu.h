#pragma once
#include "../Benchmark.h"

#include "pico/rand.h"
#include "pico/time.h"
#include <iostream>

#include <nlohmann/json.hpp>

#include <Eigen/Dense>

template <typename P> class LU : public Benchmark {
private:
  uint32_t min_dim;
  uint32_t max_dim;

public:
  LU(uint32_t min_dim, uint32_t max_dim) : max_dim(max_dim), min_dim(min_dim) {}
  void run(bool debug) override;

  void sof() override;
};
template <typename P> void LU<P>::sof() {
  printf("SOF\n");
  printf("X,Y,time_us\n");
}

// after this example:
// https://libeigen.gitlab.io/eigen/docs-5.0/classEigen_1_1FullPivLU.html
template <typename P> void LU<P>::run(bool debug) {
#define Dyn Eigen::Dynamic
#define GenericMatrix Eigen::Matrix<P, Dyn, Dyn>

  uint32_t x = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);
  uint32_t y = debug ? 5 : Benchmark::random_range_32(min_dim, max_dim);

  GenericMatrix M(x, y);
  GenericMatrix L(GenericMatrix::Identity(x, x));
  // random values
  M.setRandom();

  // start clock
  absolute_time_t startTime = get_absolute_time();

  Eigen::FullPivLU<GenericMatrix> lu(M);
  L.block(0, 0, x, y).template triangularView<Eigen::StrictlyLower>() =
      lu.matrixLU();
  GenericMatrix U(lu.matrixLU().template triangularView<Eigen::Upper>());

  // stop clock
  absolute_time_t stopTime = get_absolute_time();

  // prevent optimization (hopefully)
  volatile P sinkU = U(0, 0);
  (void)sinkU;
  volatile P sinkL = L(0, 0);
  (void)sinkL;

  uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
  if (!debug) {
    printf("%lu,%lu,%llu\n", x, y, time_us);
  }

  // debug results
  if (debug) {
    std::cout << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
              << std::endl;

    std::cout << "M = " << std::endl << M << std::endl;
    std::cout << "L = " << std::endl << L << std::endl;
    std::cout << "U = " << std::endl << U << std::endl;

    // reverse engineer this
    GenericMatrix approxM(lu.permutationP().inverse() * L * U *
                          lu.permutationQ().inverse());

    double error = (M - approxM).squaredNorm();

    std::cout << "Error = " << error << std::endl;

    std::cout << "Time µs = " << time_us << std::endl;

    std::cout << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
              << std::endl;
  }
}
