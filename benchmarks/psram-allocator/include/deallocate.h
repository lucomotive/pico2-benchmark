#pragma once

#include "pico/time.h"
#include "ps_ram.h"
#include <cstdio>
#include <tuple>

template <typename P> int64_t deallocate(P *ptr) {
  const auto start = get_absolute_time();
  psram::ps_free(ptr);
  const auto stop = get_absolute_time();
  return absolute_time_diff_us(start, stop);
}
