#pragma once

#include <cstdint>

#if defined(BUILD_PICO)
#include <pico/time.h>
#include <pico/types.h>
#elif defined(BUILD_HOST)
#include <chrono>
#endif

namespace my_timer {

using Time = int64_t;

class Timer {
private:
#if defined(BUILD_PICO)
  using _time_t = absolute_time_t;
#elif defined(BUILD_HOST)
  using _time_t = std::chrono::time_point<steady_clock>;
#endif

  _time_t start;

public:
  Timer();

  [[nodiscard]] Time elapsed_us() const;
};

}; // namespace my_timer
