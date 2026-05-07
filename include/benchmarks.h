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
const uint32_t iterations_default = 1000;
const uint32_t min_default = 5;
const uint32_t max_default = 100;
#endif
const uint32_t step_default = 1;
const float dim_ratio_default = 0.3;
const float step_growth_default = 0.0;
const char *precision_default = "float";

template <typename P>
using GenericMatrix = Eigen::Matrix<P, Eigen::Dynamic, Eigen::Dynamic>;

const uint32_t random_range_32(uint32_t low, uint32_t high) {
  return low + (get_rand_32() % (high - low));
}

template <bool Debug, typename P> void inverse(const nlohmann::json &json) {
  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);

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
    volatile P sink = R(random_range_32(0, size), random_range_32(0, size));
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
    benchmark(4);
    benchmark(7);
    benchmark(12);
    return;
  }

  printf("size,time_us\n");
  for (int i = min; i <= max; i += (int)step) {
    printf("%lu,%llu\n", i, benchmark(i));
    step += step_growth;
  }
}

template <bool Debug, typename P> void determinant(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  auto benchmark = [&](uint32_t size) -> uint64_t {
    GenericMatrix<P> A(size, size);
    // random values
    A.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    volatile P res = A.determinant();

    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sink = A(random_range_32(0, size), random_range_32(0, size));
    (void)sink;
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
    benchmark(4);
    benchmark(7);
    benchmark(12);
    return;
  }

  printf("size,time_us\n");
  for (int i = min; i <= max; i += (int)step) {
    printf("%lu,%llu\n", i, benchmark(i));
    step += step_growth;
  }
}

template <bool Debug, typename P> void lu(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

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
    volatile P sinkU = U(random_range_32(0, x), random_range_32(0, y));
    (void)sinkU;
    volatile P sinkL = L(random_range_32(0, x), random_range_32(0, x));
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
    benchmark(7, 4);
    benchmark(5, 5);
    benchmark(12, 3);
    return;
  }

  printf("X,Y,time_us\n");
  for (int x = min; x < max; x += (int)step) {
    for (int y = min; y < x; y += (int)sub_step) {
      if (y < x * dim_ratio /*||x < y * skip not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
    }
    step += step_growth;
    sub_step += sub_step_growth;
  }
}

template <bool Debug, typename P> void qr(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

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
    volatile P sink = R(random_range_32(0, y), random_range_32(0, y));
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
    benchmark(4, 7);
    benchmark(5, 5);
    benchmark(7, 4);
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
  for (int x = min; x <= max; x += (int)step) {
    for (int y = min; y <= x; y += (int)sub_step) {
      if (y < x * dim_ratio /*|| x < y * skip not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
      if (x != y)
        printf("%lu,%lu,%llu\n", y, x, benchmark(y, x));
    }
    step += step_growth;
    sub_step += sub_step_growth;
  }
}

template <bool Debug, typename P> void matmul(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

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
    volatile P sink = R(random_range_32(0, x), random_range_32(0, z));
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
    benchmark(5, 5, 5);
    benchmark(7, 3, 6);
    benchmark(13, 8, 4);
    return;
  }

  printf("X,Y,Z,time_us\n");
  for (int x = min; x <= max; x += (int)step) {
    for (int y = min; y <= x; y += (int)sub_step) {
      for (int z = min; z <= y; z += (int)sub_step) {
        if (y < x * dim_ratio || z < y * dim_ratio ||
            z < x * dim_ratio /*|| x < y * skip not possible*/)
          continue;
        printf("%lu,%lu,%lu,%llu\n", x, y, z, benchmark(x, y, z));
        if (x != y)
          printf("%lu,%lu,%lu,%llu\n", y, x, z, benchmark(y, x, z));
        if (y != z)
          printf("%lu,%lu,%lu,%llu\n", x, z, y, benchmark(x, z, y));
        if (z != x)
          printf("%lu,%lu,%lu,%llu\n", z, y, x, benchmark(z, y, x));
      }
    }
    step += step_growth;
    sub_step += sub_step_growth;
  }
}

template <bool Debug, typename P> void heap_alloc(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  auto benchmark = [&](uint32_t size) {
    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    GenericMatrix<P> A(size, size);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    A.setRandom();
    volatile P sink = A(random_range_32(0, size), random_range_32(0, size));
    (void)sink;

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug)
    return;

  printf("size,time_us\n");
  for (int i = min; i < max; i += (int)step) {
    printf("%lu,%llu\n", i, benchmark(i));
    step += step_growth;
  }
}

template <bool Debug, typename P> void stack_alloc(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json.value("precision", precision_default);

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
    volatile P sink =
        A(random_range_32(0, max_default), random_range_32(0, max_default));
    (void)sink;

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug)
    return;

  // printf("SOF result %s %s\n", name.c_str(), precision.c_str());
  printf("size,time_us\n");
  for (int i = 0; i < iterations; i++) {
    printf("%lu,%llu\n", max_default, benchmark());
  }
  // printf("EOF result %s %s\n", name.c_str(), precision.c_str());
  // printf("EOT\n");
}

// copies a matrix from heap to heap
template <bool Debug, typename P> void copy(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json.value("precision", precision_default);

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

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
    volatile P sink = R(random_range_32(0, x), random_range_32(0, y));
    (void)sink;

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug)
    return;

  // printf("SOF result %s %s\n", name.c_str(), precision.c_str());
  printf("X,Y,time_us\n");
  for (int x = min; x <= max; x += (int)step) {
    for (int y = min; y <= x; y += (int)sub_step) {
      // cut out huge ratios
      if (y < x * dim_ratio /*|| x < y * skip not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
      if (y != x)
        printf("%lu,%lu,%llu\n", y, x, benchmark(y, x));
    }
    step += step_growth;
    sub_step += sub_step_growth;
  }
  // printf("EOF result %s %s\n", name.c_str(), precision.c_str());
  // printf("EOT\n");
}

// Generate a compile-time random array
template <typename P, uint32_t X, uint32_t Y>
constexpr std::array<P, X * Y> make_random_array() {
  const auto lcg_next = [](uint32_t s) { return s * 1664525u + 1013904223u; };
  auto gen_float = [](uint32_t s) { return s * (1.0 / 4294967296.0); };
  std::array<P, X * Y> arr{};
  uint32_t s = 0; // seed
  for (uint32_t i = 0; i < X * Y; i++) {
    s = lcg_next(s);
    arr[i] = (P)gen_float(s);
  }
  return arr;
}
// initialize data in flash
/* seems to compile correctly to flash:
 arm-none-eabi-nm --size-sort --print-size build/pico-benchmark.elf | c++filt |
 grep data\<
 */
template <typename P, uint32_t X, uint32_t Y>
const std::array<P, X * Y> data = make_random_array<P, X, Y>();
//  copies a const matrix from flash to heap
template <bool Debug, typename P> void read_flash(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json.value("precision", precision_default);

  uint32_t iterations = json.value("iterations", min_default);

  auto benchmark = [&]<uint32_t X, uint32_t Y>() {
    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    const Eigen::Map<const GenericMatrix<P>> R(data<P, X, Y>.data(), X, Y);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sink = R(random_range_32(0, X), random_range_32(0, Y));
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "R = " << std::endl << R << std::endl;
      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }

    return time_us;
  };
  if constexpr (Debug) {
    benchmark.template operator()<6, 2>();
    benchmark.template operator()<3, 3>();
    benchmark.template operator()<5, 4>();
    return;
  }

  // printf("SOF result %s %s\n", name.c_str(), precision.c_str());
  printf("size,time_us\n");
  for (int x = 0; x < iterations; x++) {
    printf("%lu,%llu\n", x, benchmark.template operator()<200, 200>());
  }
  // printf("EOF result %s %s\n", name.c_str(), precision.c_str());
  // printf("EOT\n");
}

template <bool Debug, typename P> void eigen(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json.value("precision", precision_default);

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);

  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  auto benchmark = [&](uint32_t size) -> std::pair<uint64_t, bool> {
    GenericMatrix<P> A(size, size);
    A.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    Eigen::SelfAdjointEigenSolver<GenericMatrix<P>> eigensolver(A);

    // stop clock
    absolute_time_t stopTime_1 = get_absolute_time();
    uint64_t time_1_us = absolute_time_diff_us(startTime, stopTime_1);

    // if not existant, exit
    if (eigensolver.info() != Eigen::Success) {
      if constexpr (Debug) {
        std::cout
            << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
            << std::endl;

        std::cout << "A = " << std::endl << A << std::endl;
        std::cout << "Time µs = " << time_1_us << std::endl;
        std::cout << "Not computable" << std::endl;

        std::cout
            << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
            << std::endl;
      }

      return {time_1_us, false};
    }

    auto values = eigensolver.eigenvalues();
    auto vectors = eigensolver.eigenvectors();

    // stop clock
    absolute_time_t stopTime_2 = get_absolute_time();
    uint64_t time_2_us = absolute_time_diff_us(startTime, stopTime_2);

    // prevent optimization (hopefully)
    volatile P sinkVal = values(random_range_32(0, size));
    (void)sinkVal;
    volatile P sinkVec =
        vectors(random_range_32(0, size), random_range_32(0, size));
    (void)sinkVec;

    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "A = " << std::endl << A << std::endl;
      std::cout << "Values = " << std::endl << values << std::endl;
      std::cout << "Vectors = " << std::endl << vectors << std::endl;

      std::cout << "Time µs = " << time_2_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }

    return {time_2_us, true};
  };
  if constexpr (Debug) {
    benchmark(5);
    benchmark(12);
    benchmark(7);
    return;
  }

  // printf("SOF result %s %s\n", name.c_str(), precision.c_str());
  printf("size,time_us,computable\n");
  for (int x = min; x < max; x += step) {
    auto [time_us, computable] = benchmark(x);
    printf("%lu,%llu,%s\n", x, time_us, computable ? "true" : "false");
    step += step_growth;
  }
  // printf("EOF result %s %s\n", name.c_str(), precision.c_str());
  // printf("EOT\n");
}

template <bool Debug, typename P> void rank(const nlohmann::json &json) {
  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  float step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

  auto benchmark = [&](uint32_t x, uint32_t y) -> uint64_t {
    GenericMatrix<P> A(x, y);
    A.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    // perform calculation
    Eigen::FullPivLU<GenericMatrix<P>> lu(A);
    volatile auto rank = lu.rank();

    // stop clock
    absolute_time_t stopTime = get_absolute_time();
    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

    // prevent optimization (hopefully)
    (void)rank;

    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "A = " << std::endl << A << std::endl;
      std::cout << "Rank = " << std::endl << rank << std::endl;
      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }

    return time_us;
  };
  if constexpr (Debug) {
    benchmark(5, 3);
    benchmark(12, 7);
    benchmark(3, 7);
    return;
  }

  printf("X,Y,time_us\n");
  for (int x = min; x <= max; x += (int)step) {
    for (int y = min; y <= x; y += (int)sub_step) {
      if (y < x * dim_ratio /*|| x < y * skip not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
      if (x != y)
        printf("%lu,%lu,%llu\n", y, x, benchmark(y, x));
    }
    step += step_growth;
    sub_step += sub_step_growth;
  }
}
