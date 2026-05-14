#pragma once

#include <pico/rand.h>

template <typename P> P random_int_range(P low, P high) {
  return low + ((P)get_rand_64() % (high - low));
}

template <typename P> P random_float() {
  const uint32_t max_int = (uint32_t)0 - (uint32_t)1;
  return (P)get_rand_32() * (1.0 / (P)max_int);
}
