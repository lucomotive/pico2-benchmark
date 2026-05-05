#pragma once

#include "pico/rand.h"
#include "pico/time.h"

#include <Eigen/Dense>
#include <iostream>
#include <nlohmann/json.hpp>

#if defined(PICO_RP2350)
const uint32_t iterations_default = 1000;
const uint32_t min_default = 5;
const uint32_t max_default = 100;
#elif defined(PICO_RP2040)
// todo
#endif

template <typename P>
using GenericMatrix = Eigen::Matrix<P, Eigen::Dynamic, Eigen::Dynamic>;

static uint32_t random_range_32(uint32_t low, uint32_t high) {
  return low + (get_rand_32() % (high - low + 1));
}

template <bool Debug, typename P> void inverse(const nlohmann::json &json) {
  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  auto benchmark = [&](uint32_t size) -> uint64_t {
    GenericMatrix<P> M(size, size);
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

    return time_us;
  };
  if constexpr (Debug) {
    benchmark(min);
    return;
  }

  printf("size,time_us\n");
  for (int i = min; i <= max; i++) {
    printf("%lu,%llu\n", i, benchmark(i));
  }
}

template <bool Debug, typename P> void determinant(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  auto benchmark = [&](uint32_t size) -> uint64_t {
    GenericMatrix<P> A(size, size);
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
    }
    return time_us;
  };
  if constexpr (Debug) {
    benchmark(min);
    return;
  }

  printf("size,time_us\n");
  for (int i = min; i <= max; i++) {
    printf("%lu,%llu\n", i, benchmark(i));
  }
}

template <bool Debug, typename P> void lu(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  auto benchmark = [&](uint32_t x, uint32_t y) -> uint64_t {
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
    }
    return time_us;
  };

  if constexpr (Debug) {
    benchmark(min, min);
    return;
  }

  printf("X,Y,time_us\n");
  for (int x = min; x < max; x++) {
    for (int y = min; y < x; y++) {
      if (3 * y < x /*|| 3 * x < y not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
    }
  }
}

template <bool Debug, typename P> void qr(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  auto benchmark = [&](uint32_t x, uint32_t y) -> uint64_t {
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
    }

    // if (!Debug) {
    //   printf("%lu,%lu,%llu\n", x, y, time_us);
    // }
    return time_us;
  };

  if constexpr (Debug) {
    benchmark(min, min);
    return;
  }

  // this loop counts variables x and y like this:
  // 1x1
  // 2x1
  // 1x2
  // 2x2
  // 3x1
  // 3x2
  // 3x3
  // 1x3
  // 2x3
  // ...
  //
  // this avoids an OOM error benchmarks like 5x124
  printf("X,Y,time_us\n");
  for (int x = min; x <= max; x++) {
    for (int y = min; y <= x; y++) {
      if (3 * y < x /*|| 3 * x < y not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
      if (x != y)
        printf("%lu,%lu,%llu\n", y, x, benchmark(y, x));
    }
  }
}

template <bool Debug, typename P> void matmul(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  auto benchmark = [&](uint32_t x, uint32_t y, uint32_t z) -> uint64_t {
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
    }

    return time_us;
  };
  if constexpr (Debug) {
    benchmark(min, min, min);
    return;
  }

  printf("X,Y,Z,time_us\n");
  for (int x = min; x <= max; x++) {
    for (int y = min; y <= x; y++) {
      for (int z = min; z <= y; z++) {
        printf("%lu,%lu,%lu,%llu\n", x, y, z, benchmark(x, y, z));
        if (x != y)
          printf("%lu,%lu,%lu,%llu\n", y, x, z, benchmark(y, x, z));
        if (y != z)
          printf("%lu,%lu,%lu,%llu\n", x, z, y, benchmark(x, z, y));
        if (z != x)
          printf("%lu,%lu,%lu,%llu\n", z, y, x, benchmark(z, y, x));
      }
    }
  }
}

template <bool Debug, typename P> void heap_alloc(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  auto benchmark = [&](uint32_t size) {
    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    GenericMatrix<P> A(size, size);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    A.setRandom();
    volatile P sink = A(0, 0);
    (void)sink;

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug) {
    benchmark(min);
    return;
  }

  printf("size,time_us\n");
  for (int i = min; i < max; i++) {
    printf("%lu,%llu\n", i, benchmark(i));
  }
}

template <bool Debug, typename P> void stack_alloc(const nlohmann::json &json) {

  uint32_t iterations = json.value("iterations", 200);

  auto benchmark = [&]() {
    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    Eigen::Matrix<P, max_default, max_default> A;
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    A.setRandom();
    volatile P sink = A(0, 0);
    (void)sink;

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug) {
    benchmark();
    return;
  }

  printf("size,time_us\n");
  for (int i = 0; i < iterations; i++) {
    printf("%lu,%llu\n", max_default, benchmark());
  }
}

template <bool Debug, typename P> void copy(const nlohmann::json &json) {
  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  auto benchmark = [&](uint32_t x, uint32_t y) {
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

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug) {
    benchmark(min, min);
    return;
  }

  printf("X,Y,time_us\n");
  for (int x = min; x <= max; x++) {
    for (int y = min; y <= x; y++) {
      // cut out huge ratios
      if (3 * y < x /*|| 3 * x < y not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
      if (y != x)
        printf("%lu,%lu,%llu\n", y, x, benchmark(y, x));
    }
  }
}
