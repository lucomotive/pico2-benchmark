

#include "timer.h"

namespace my_timer {

#if defined(BUILD_PICO)
Timer::Timer() : start(get_absolute_time()) {}

[[nodiscard]] Time Timer::elapsed_us() const {
  auto now = get_absolute_time();
  return absolute_time_diff_us(start, now);
}
#elif defined(BUILD_HOST)
Timer::Timer() : start(steady_clock::now()) {}

[[nodiscard]] Time Timer::elapsed_us() const {
  auto now = steady_clock::now();

  using us = std::chrono::microseconds;
  const auto t_us = std::chrono::duration_cast<us>(now - start);
  // const double us_per_s = 100.0;
  return t_us.count();
}
#endif

}; // namespace my_timer
