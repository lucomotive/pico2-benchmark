#pragma once

#include <cstdint>

#if defined(BUILD_PICO)
#include <pico/time.h>
#include <pico/types.h>
#elif defined(BUILD_HOST)
#include <chrono>
#endif

using Time = int64_t;

#if defined(BUILD_PICO)
class Timer {
private:
  absolute_time_t start;

public:
  Timer() : start(get_absolute_time()) {}

  [[nodiscard]] Time elapsed_us() const {
    auto now = get_absolute_time();
    return absolute_time_diff_us(start, now);
  }
};

#elif defined(BUILD_HOST)

using namespace std::chrono;

class Timer {
private:
  time_point<steady_clock> start;

public:
  Timer() : start(steady_clock::now()) {}

  [[nodiscard]] Time elapsed_us() const {
    auto now = steady_clock::now();

    using us = std::chrono::microseconds;
    const auto t_us = std::chrono::duration_cast<us>(now - start);
    // const double us_per_s = 100.0;
    return t_us.count();
  }
};

#endif
