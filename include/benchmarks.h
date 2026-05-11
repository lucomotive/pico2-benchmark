#pragma once

#include "pico/rand.h"
#include "pico/time.h"

#include <Eigen/Dense>
#include <Eigen/src/Cholesky/LLT.h>
#include <Eigen/src/Core/Matrix.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>

#include "const_rand.h"

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

const size_t DEBUG_BENCHMARK_COUNT = 3;
const size_t DEBUG_BENCHMARK_MIN_DIM = 3;
const size_t DEBUG_BENCHMARK_MAX_DIM = 10;

template <typename P>
using GenericMatrix = Eigen::Matrix<P, Eigen::Dynamic, Eigen::Dynamic>;
template <typename P> using GenericVector = Eigen::Vector<P, Eigen::Dynamic>;

uint32_t random_range_32(uint32_t low, uint32_t high) {
  return low + (get_rand_32() % (high - low));
}

template <bool Debug, typename P> void inverse(const nlohmann::json &json) {
  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  auto step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  auto benchmark = [&](uint16_t size) -> uint64_t {
    GenericMatrix<P> source(size, size);
    // random values
    source.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    GenericMatrix<P> result(source.inverse());

    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevents optimization
    volatile P sink =
        result(random_range_32(0, size), random_range_32(0, size));
    (void)sink;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

    // debug results
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "M = " << std::endl << source << std::endl;
      std::cout << "R = " << std::endl << result << std::endl;
      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }

    return time_us;
  };
  if constexpr (Debug) {
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
    return;
  }

  printf("size,time_us\n");
  for (uint16_t i = min; i <= max; i += (uint16_t)step) {
    printf("%lu,%llu\n", i, benchmark(i));
    step += step_growth;
  }
}

template <bool Debug, typename P> void determinant(const nlohmann::json &json) {

  uint32_t min = json.value("min-dimension", min_default);
  uint32_t max = json.value("max-dimension", max_default);
  auto step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  auto benchmark = [&](uint16_t size) -> uint64_t {
    GenericMatrix<P> source(size, size);
    // random values
    source.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    volatile P res = source.determinant();

    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sink =
        source(random_range_32(0, size), random_range_32(0, size));
    (void)sink;
    (void)res;

    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

    // debug results
    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "A = " << std::endl << source << std::endl;
      std::cout << "R = " << std::endl << res << std::endl;

      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }
    return time_us;
  };
  if constexpr (Debug) {
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
    return;
  }

  printf("size,time_us\n");
  for (uint32_t i = min; i <= max; i += (int)step) {
    printf("%lu,%llu\n", i, benchmark(i));
    step += step_growth;
  }
}

template <bool Debug, typename P> void lu(const nlohmann::json &json) {

  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  auto step = (float)json.value("step", step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  auto sub_step = (float)json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

  auto benchmark = [&](uint16_t x, uint16_t y) -> uint64_t {
    GenericMatrix<P> source(x, y);
    // random values
    source.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    GenericMatrix<P> L(GenericMatrix<P>::Identity(x, x));

    Eigen::FullPivLU<GenericMatrix<P>> lu(source);
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

      std::cout << "M = " << std::endl << source << std::endl;
      std::cout << "L = " << std::endl << L << std::endl;
      std::cout << "U = " << std::endl << U << std::endl;

      // reverse engineer this
      GenericMatrix<P> reverse(lu.permutationP().inverse() * L * U *
                               lu.permutationQ().inverse());

      double error = (source - reverse).squaredNorm();

      std::cout << "Error = " << error << std::endl;

      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }
    return time_us;
  };

  if constexpr (Debug) {
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM),
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
    return;
  }

  printf("X,Y,time_us\n");
  for (uint16_t x = min; x < max; x += (uint16_t)step) {
    for (uint16_t y = min; y < x; y += (uint16_t)sub_step) {
      if ((float)y < (float)x * dim_ratio /*||x < y * skip not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
    }
    step += step_growth;
    sub_step += sub_step_growth;
  }
}

template <bool Debug, typename P> void qr(const nlohmann::json &json) {

  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  float step = json.value("step", (float)step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

  auto benchmark = [&](uint16_t x, uint16_t y) -> uint64_t {
    GenericMatrix<P> source(x, y);
    // random values
    source.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    Eigen::HouseholderQR<GenericMatrix<P>> qr(source);
    GenericMatrix<P> thinQ(GenericMatrix<P>::Identity(x, y));

    GenericMatrix<P> Q(qr.householderQ() * thinQ);
    GenericMatrix<P> R(Q.transpose() * source);

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

      std::cout << "A = " << std::endl << source << std::endl;
      std::cout << "Q = " << std::endl << Q << std::endl;
      std::cout << "R = " << std::endl << R << std::endl;

      // reverse engineer this
      GenericMatrix<P> reverse(Q * R);
      double error = (source - reverse).squaredNorm();
      std::cout << "A ≈ " << std::endl << reverse << std::endl;
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
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM),
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
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
  for (uint16_t x = min; x <= max; x += (int)step) {
    for (uint16_t y = min; y <= x; y += (int)sub_step) {
      if ((float)y < (float)x * dim_ratio /*|| x < y * skip not possible*/)
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

  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  float step = json.value("step", (float)step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

  auto benchmark = [&](uint16_t x, uint16_t y, uint16_t z) -> uint64_t {
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
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM),
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM),
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
    return;
  }

  printf("X,Y,Z,time_us\n");
  for (uint16_t x = min; x <= max; x += (uint16_t)step) {
    for (uint16_t y = min; y <= x; y += (uint16_t)sub_step) {
      for (uint16_t z = min; z <= y; z += (uint16_t)sub_step) {
        if ((float)y < (float)x * dim_ratio ||
            (float)z < (float)y * dim_ratio ||
            (float)z < (float)x * dim_ratio /*|| x < y * skip not possible*/)
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

  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  float step = json.value("step", (float)step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  auto benchmark = [&](uint16_t size) {
    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    GenericMatrix<P> source(size, size);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    source.setRandom();
    volatile P sink =
        source(random_range_32(0, size), random_range_32(0, size));
    (void)sink;

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug) {
    printf("Nothing interesting to show");
    return;
  }

  printf("size,time_us\n");
  for (int i = min; i < max; i += (int)step) {
    printf("%lu,%llu\n", i, benchmark(i));
    step += step_growth;
  }
}

template <bool Debug, typename P> void stack_alloc(const nlohmann::json &json) {
  uint16_t iterations = json.value("iterations", iterations_default);

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
  if constexpr (Debug) {
    printf("Nothing interesting to show");
    return;
  }

  // printf("SOF result %s %s\n", name.c_str(), precision.c_str());
  printf("size,time_us\n");
  for (uint16_t i = 0; i < iterations; i++) {
    printf("%lu,%llu\n", max_default, benchmark());
  }
  // printf("EOF result %s %s\n", name.c_str(), precision.c_str());
  // printf("EOT\n");
}

// copies a matrix from heap to heap
template <bool Debug, typename P> void copy(const nlohmann::json &json) {

  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  float step = json.value("step", (float)step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

  auto benchmark = [&](uint16_t x, uint16_t y) {
    GenericMatrix<P> source(x, y);
    // random values
    source.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    GenericMatrix<P> copy(source);
    // stop clock
    absolute_time_t stopTime = get_absolute_time();

    // prevent optimization (hopefully)
    volatile P sink = copy(random_range_32(0, x), random_range_32(0, y));
    (void)sink;

    return absolute_time_diff_us(startTime, stopTime);
  };
  if constexpr (Debug) {
    printf("Nothing interesting to show");
    return;
  }

  // printf("SOF result %s %s\n", name.c_str(), precision.c_str());
  printf("X,Y,time_us\n");
  for (uint16_t x = min; x <= max; x += (uint16_t)step) {
    for (uint16_t y = min; y <= x; y += (uint16_t)sub_step) {
      // cut out huge ratios
      if ((float)y < (float)x * dim_ratio /*|| x < y * skip not possible*/)
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

// initialize data in flash
/* seems to compile correctly to flash:
 arm-none-eabi-nm --size-sort --print-size build/pico-benchmark.elf | c++filt |
 grep data\<
 */
template <typename P, uint32_t X, uint32_t Y>
const std::array<P, X * Y> data = ConstRand::random_float_array<P, X * Y>();

/*template <uint32_t N, typename F> void repeat(F &&f) {
  [&]<uint32_t... Is>(std::integer_sequence<uint32_t, Is...>) {
    (f.template operator()<Is>(), ...);
  }(std::make_integer_sequence<uint32_t, N>{});
}*/
template <uint32_t N, typename F> constexpr void repeat(F &&f) {
  if constexpr (N > 0) {
    f.template operator()<N - 1>();
    repeat<N - 1>(std::forward<F>(f));
  }
}
//  copies a const matrix from flash to heap
template <bool Debug, typename P> void read_flash(const nlohmann::json &json) {

  uint16_t iterations = json.value("iterations", iterations_default);

  auto benchmark = [&]<uint16_t X, uint16_t Y>() {
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
    constexpr auto dimensions =
        ConstRand::random_int_array<uint32_t, DEBUG_BENCHMARK_COUNT * 2>(3, 10);
    auto f = [&]<uint32_t n>() {
      benchmark.template
      operator()<dimensions[n], dimensions[DEBUG_BENCHMARK_COUNT + n]>();
    };
    repeat<DEBUG_BENCHMARK_COUNT>(f);

    return;
  }

  // printf("SOF result %s %s\n", name.c_str(), precision.c_str());
  printf("size,time_us\n");
  for (uint16_t x = 0; x < iterations; x++) {
    printf("%lu,%llu\n", x, benchmark.template operator()<200, 200>());
  }
  // printf("EOF result %s %s\n", name.c_str(), precision.c_str());
  // printf("EOT\n");
}

template <bool Debug, typename P> void eigen(const nlohmann::json &json) {

  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);

  float step = json.value("step", (float)step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  auto benchmark = [&](uint16_t size) -> std::tuple<uint64_t, bool> {
    GenericMatrix<P> A(size, size);
    A.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();
    // perform calculation
    Eigen::SelfAdjointEigenSolver<GenericMatrix<P>> eigensolver(A);

    // if not existant, exit
    if (eigensolver.info() != Eigen::Success) {
      // stop clock
      absolute_time_t stopTime = get_absolute_time();
      uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

      if constexpr (Debug) {
        std::cout
            << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
            << std::endl;

        std::cout << "A = " << std::endl << A << std::endl;
        std::cout << "Time µs = " << time_us << std::endl;
        std::cout << "Not computable" << std::endl;

        std::cout
            << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
            << std::endl;
      }

      return {time_us, false};
    }

    auto values = eigensolver.eigenvalues();
    auto vectors = eigensolver.eigenvectors();

    // stop clock
    absolute_time_t stopTime = get_absolute_time();
    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

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

      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }

    return {time_us, true};
  };
  if constexpr (Debug) {
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
    return;
  }

  printf("size,time_us,computable\n");
  for (uint16_t x = min; x < max; x += (uint16_t)step) {
    auto [time_us, computable] = benchmark(x);
    printf("%lu,%llu,%d\n", x, time_us, computable);
    step += step_growth;
  }
}

template <bool Debug, typename P> void rank(const nlohmann::json &json) {
  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  float step = json.value("step", (float)step_default);
  float step_growth = json.value("step-growth", step_growth_default);
  float sub_step = json.value("sub-step", step);
  float sub_step_growth = json.value("sub-step-growth", step_growth);

  float dim_ratio = json.value("max-dim-ratio", dim_ratio_default);

  auto benchmark = [&](uint16_t x, uint16_t y) -> uint64_t {
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
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM),
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
    return;
  }

  printf("X,Y,time_us\n");
  for (uint16_t x = min; x <= max; x += (uint16_t)step) {
    for (uint16_t y = min; y <= x; y += (uint16_t)sub_step) {
      if ((float)y < (float)x * dim_ratio /*|| x < y * skip not possible*/)
        continue;
      printf("%lu,%lu,%llu\n", x, y, benchmark(x, y));
      if (x != y)
        printf("%lu,%lu,%llu\n", y, x, benchmark(y, x));
    }
    step += step_growth;
    sub_step += sub_step_growth;
  }
}

// after this example:
// https://libeigen.gitlab.io/eigen/docs-3.4/group__TopicSparseSystems.html#TheSparseSolve
template <bool Debug, typename P> void llt(const nlohmann::json &json) {
  uint16_t min = json.value("min-dimension", min_default);
  uint16_t max = json.value("max-dimension", max_default);
  float step = json.value("step", (float)step_default);
  float step_growth = json.value("step-growth", step_growth_default);

  // returns tuple{time, decomposition, solved}
  auto benchmark = [&](uint16_t size) -> std::tuple<uint64_t, bool, bool> {
    GenericMatrix<P> A(size, size);
    A.setRandom();
    // A must be SPD matrix
    A = (A.transpose() * A) +
        (GenericMatrix<P>::Identity(size, size) * (P)size);

    GenericVector<P> B(size);
    B.setRandom();

    // start clock
    absolute_time_t startTime = get_absolute_time();

    Eigen::LLT<GenericMatrix<P>> solver;
    solver.compute(A);
    if (solver.info() != Eigen::Success) {
      // decomposition failed
      absolute_time_t stopTime = get_absolute_time();
      uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

      if constexpr (Debug) {
        std::cout
            << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
            << std::endl;

        std::cout << "A = " << std::endl << A << std::endl;
        std::cout << "B = " << std::endl << B << std::endl;
        std::cout << "Decomposition failed" << std::endl;
        std::cout << "Time µs = " << time_us << std::endl;

        std::cout
            << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
            << std::endl;
      }

      return {time_us, false, false};
    }
    auto res = solver.solve(B);
    if (solver.info() != Eigen::Success) {
      // solving failed
      absolute_time_t stopTime = get_absolute_time();
      uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

      if constexpr (Debug) {
        std::cout
            << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
            << std::endl;

        std::cout << "A = " << std::endl << A << std::endl;
        std::cout << "B = " << std::endl << B << std::endl;
        std::cout << "Solving failed" << std::endl;
        std::cout << "Time µs = " << time_us << std::endl;

        std::cout
            << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
            << std::endl;
      }

      return {time_us, true, false};
    }

    // stop clock
    absolute_time_t stopTime = get_absolute_time();
    uint64_t time_us = absolute_time_diff_us(startTime, stopTime);

    // prevent optimization (hopefully)
    volatile P sink = res(random_range_32(0, size));
    (void)sink;

    if constexpr (Debug) {
      std::cout
          << "–––––––––––––––––––––Testing algorithm––––––––––––––––––––––"
          << std::endl;

      std::cout << "A = " << std::endl << A << std::endl;
      std::cout << "B = " << std::endl << B << std::endl;
      std::cout << "R = " << std::endl << res << std::endl;
      std::cout << "Time µs = " << time_us << std::endl;

      std::cout
          << "––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––"
          << std::endl;
    }

    return {
        time_us,
        true,
        true,
    };
  };
  if constexpr (Debug) {
    for (uint32_t i = 0; i < DEBUG_BENCHMARK_COUNT; i++) {
      benchmark(
          random_range_32(DEBUG_BENCHMARK_MIN_DIM, DEBUG_BENCHMARK_MAX_DIM));
    }
    return;
  }

  printf("size,time_us,decomposition,solved\n");
  for (uint16_t x = min; x <= max; x += (uint16_t)step) {
    auto [time, dec, sol] = benchmark(x);
    printf("%lu,%llu,%d,%d\n", x, time, dec, sol);
  }
  step += step_growth;
}
