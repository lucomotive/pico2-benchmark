#pragma once

#include <cstdint>
#include <cstdlib>
#if defined(BUILD_PICO)
#include <pico/rand.h>
#endif

namespace my_rand {

uint64_t random_64() {
#if defined(BUILD_PICO)
  return get_rand_64();
#elif defined(BUILD_HOST)
  return std::rand();
#endif
}

template <typename P> P random_int_range(P low, P high) {
  return low + ((P)random_64() % (high - low));
}

template <typename P> P random_float() {
  const uint32_t max_int = (uint32_t)0 - (uint32_t)1;
  return (P)random_64() * (1.0 / (P)max_int);
}

}; // namespace my_rand
