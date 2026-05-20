#pragma once

#include <pico/time.h>
#include <pico/types.h>

using Time = int64_t;

class Timer {
private:
  absolute_time_t start;

public:
  Timer() : start(get_absolute_time()) {}

  [[nodiscard]] Time elapsed() const {
    auto now = get_absolute_time();
    return absolute_time_diff_us(start, now);
  }
};
