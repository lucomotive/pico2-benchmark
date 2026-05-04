#pragma once

#include "pico/rand.h"
#include "pico/time.h"

#include <Eigen/Dense>
#include <iostream>
#include <nlohmann/json.hpp>

template <typename P>
using GenericMatrix = Eigen::Matrix<P, Eigen::Dynamic, Eigen::Dynamic>;

static uint32_t random_range_32(uint32_t low, uint32_t high) {
  return low + (get_rand_32() % (high - low + 1));
}

template <bool Debug, typename P> void inverse(const nlohmann::json &json) {
  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];

  if constexpr (!Debug) {
    printf("size,time_us\n");
  }
  for (int i = min; i < max; i++) {
    GenericMatrix<P> M(i, i);
    // random values
    M.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    GenericMatrix<P> R(M.inverse());

    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevents optimization
    volatile P sink = R(0, 0);
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if constexpr (!Debug) {
      printf("%lu,%llu\n", i, time_us);
    }

    // debug results
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "M = " << std::endl << M << std::endl;
      std::cout << "R = " << std::endl << R << std::endl;
      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }
  }
}

template <bool Debug, typename P> void determinant(const nlohmann::json &json) {

  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];

  if constexpr (!Debug) {
    printf("size,time_us\n");
  }
  for (int i = min; i < max; i++) {
    GenericMatrix<P> A(i, i);
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
    if constexpr (!Debug) {
      printf("%lu,%llu\n", i, time_us);
    }

    // debug results
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "A = " << std::endl << A << std::endl;
      std::cout << "R = " << std::endl << res << std::endl;

      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;

      break;
    }
  }
}

template <bool Debug, typename P> void lu(const nlohmann::json &json) {

  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];
  uint32_t iterations = json["iterations"];

  if constexpr (!Debug) {
    printf("X,Y,time_us\n");
  }
  for (int i = 0; i < iterations; i++) {

    uint32_t x = Debug ? 5 : random_range_32(min, max);
    uint32_t y = Debug ? 5 : random_range_32(min, x);

    GenericMatrix<P> M(x, y);
    GenericMatrix<P> L(GenericMatrix<P>::Identity(x, x));
    // random values
    M.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    Eigen::FullPivLU<GenericMatrix<P>> lu(M);
    L.block(0, 0, x, y).template triangularView<Eigen::StrictlyLower>() =
        lu.matrixLU();
    GenericMatrix<P> U(lu.matrixLU().template triangularView<Eigen::Upper>());

    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sinkU = U(0, 0);
    (void)sinkU;
    volatile P sinkL = L(0, 0);
    (void)sinkL;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if (!Debug) {
      printf("%lu,%lu,%llu\n", x, y, time_us);
    }
    // debug results
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "M = " << std::endl << M << std::endl;
      std::cout << "L = " << std::endl << L << std::endl;
      std::cout << "U = " << std::endl << U << std::endl;

      // reverse engineer this
      GenericMatrix<P> approxM(lu.permutationP().inverse() * L * U *
                               lu.permutationQ().inverse());

      double error = (M - approxM).squaredNorm();

      std::cout << "Error = " << error << std::endl;

      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;

      break;
    }
  }
}

template <bool Debug, typename P> void qr(const nlohmann::json &json) {

  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];
  uint32_t iterations = json["iterations"];

  if constexpr (!Debug) {
    printf("X,Y,time_us\n");
  }
  for (int i = 0; i < iterations; i++) {

    uint32_t x = Debug ? 5 : random_range_32(min, max);
    uint32_t y = Debug ? 5 : random_range_32(min, x);

    GenericMatrix<P> A(x, y);
    // random values
    A.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    Eigen::HouseholderQR<GenericMatrix<P>> qr(A);
    GenericMatrix<P> thinQ(GenericMatrix<P>::Identity(x, y));

    GenericMatrix<P> Q(qr.householderQ() * thinQ);
    GenericMatrix<P> R(Q.transpose() * A);

    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sink = R(0, 0);
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if (!Debug) {
      printf("%lu,%lu,%llu\n", x, y, time_us);
    }
    // debug results
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "A = " << std::endl << A << std::endl;
      std::cout << "Q = " << std::endl << Q << std::endl;
      std::cout << "R = " << std::endl << R << std::endl;

      // reverse engineer this
      GenericMatrix<P> approxA(Q * R);
      double error = (A - approxA).squaredNorm();
      std::cout << "A ≈ " << std::endl << approxA << std::endl;
      std::cout << "Error = " << error << std::endl;

      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;

      break;
    }
  }
}

template <bool Debug, typename P> void matmul(const nlohmann::json &json) {

  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];
  uint32_t iterations = json["iterations"];

  if constexpr (!Debug) {
    printf("X,Y,Z,time_us\n");
  }
  for (int i = 0; i < iterations; i++) {

    uint32_t x = Debug ? 5 : random_range_32(min, max);
    uint32_t y = Debug ? 5 : random_range_32(min, max);
    uint32_t z = Debug ? 5 : random_range_32(min, max);

    // allocate matrices
    GenericMatrix<P> M1(x, y);
    GenericMatrix<P> M2(y, z);
    // write random values
    M1.setRandom();
    M2.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    GenericMatrix<P> R(M1 * M2);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sink = R(0, 0);
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if constexpr (!Debug) {
      printf("%lu,%lu,%lu,%llu\n", x, y, z, time_us);
    }

    // debug results
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "M1 = " << std::endl << M1 << std::endl;
      std::cout << "M2 = " << std::endl << M2 << std::endl;
      std::cout << "R = " << std::endl << R << std::endl;

      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;

      break;
    }
  }
}

template <bool Debug, typename P> void heap_alloc(const nlohmann::json &json) {

  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];
  uint32_t iterations = json["iterations"];

  if constexpr (!Debug) {
    printf("X,Y,time_us\n");
  }
  for (int i = 0; i < iterations; i++) {

    uint32_t x = Debug ? 5 : random_range_32(min, max);
    uint32_t y = Debug ? 5 : random_range_32(min, max);

    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    GenericMatrix<P> A(x, y);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    A.setRandom();
    volatile P sink = A(0, 0);
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if constexpr (!Debug) {
      printf("%lu,%lu,%llu\n", x, y, time_us);
    }

    // skip debug (useless in this case)
    if constexpr (Debug)
      break;
  }
}

template <bool Debug, typename P> void stack_alloc(const nlohmann::json &json) {

  const int MATRIX_SIZE = 100;

  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];

  if constexpr (!Debug) {
    printf("size,time_us\n");
  }
  for (int i = min; i < max; i++) {

    // uint32_t x = Debug ? 5 : random_range_32(min, max);
    // uint32_t y = Debug ? 5 : random_range_32(min, max);

    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    Eigen::Matrix<P, MATRIX_SIZE, MATRIX_SIZE> A;
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    A.setRandom();
    volatile P sink = A(0, 0);
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if constexpr (!Debug) {
      printf("%lu,%llu\n", MATRIX_SIZE, time_us);
    }

    // skip debug (useless in this case)
    if constexpr (Debug)
      break;
  }
}

template <bool Debug, typename P> void copy(const nlohmann::json &json) {
  uint32_t min = json["min-dimension"];
  uint32_t max = json["max-dimension"];
  uint32_t iterations = json["iterations"];

  if constexpr (!Debug) {
    printf("X,Y,time_us\n");
  }
  for (int i = 0; i < iterations; i++) {

    uint32_t x = Debug ? 5 : random_range_32(min, max);
    uint32_t y = Debug ? 5 : random_range_32(min, max);

    GenericMatrix<P> A(x, y);
    // random values
    A.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    GenericMatrix<P> R(A);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sink = R(0, 0);
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if constexpr (!Debug) {
      printf("%lu,%lu,%llu\n", x, y, time_us);
    }

    // skip debug (useless in this case)
    if constexpr (Debug)
      break;
  }
}
