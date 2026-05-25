#pragma once

#include "pico/time.h"
#include "ps_ram.h"
#include <cstdio>
#include <tuple>

template <typename P> std::tuple<P *, int64_t> allocate(size_t size) {
  const auto start = get_absolute_time();
  auto *ptr = (P *)psram::ps_malloc(size * sizeof(P));
  const auto stop = get_absolute_time();
  return {ptr, absolute_time_diff_us(start, stop)};
}
