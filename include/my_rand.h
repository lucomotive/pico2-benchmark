#pragma once

#include <cstdint>
#include <cstdlib>
#include <type_traits>
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
  static_assert(std::is_same_v<P, float> || std::is_same_v<P, double>,
                "T must be float or double");

  if constexpr (std::is_same_v<P, float>) {
    // Use upper 23 bits for float mantissa
    return (random_64() >> 41) * (1.0f / (1u << 23));
  } else {
    // Use upper 52 bits for double mantissa
    return (random_64() >> 12) * (1.0 / (1ull << 52));
  }
  // constexpr uint64_t max_int = (uint64_t)0 - (uint64_t)1;
  // return (double)random_64() * (1.0 / (double)max_int);
}

}; // namespace my_rand
