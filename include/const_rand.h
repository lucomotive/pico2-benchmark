#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace const_rand {

using Seed = uint32_t;

constexpr uint32_t next_seed(Seed s) {
  const Seed mul = 1664525;
  const Seed add = 1013904223;
  return (s * mul) + add;
}

template <typename P> constexpr P gen_float(Seed s) {
  return s * (1.0 / (P)((Seed)0 - 1));
}

template <typename P> constexpr P gen_int(Seed s, P low, P high) {
  return low + ((P)s % (high - low));
}

// Generate a compile-time random array
template <typename P, size_t LENGTH>
constexpr std::array<P, LENGTH> random_float_array() {
  std::array<P, LENGTH> arr{};
  Seed s = 0; // seed
  for (uint32_t i = 0; i < LENGTH; i++) {
    s = next_seed(s);
    arr[i] = gen_float<P>(s);
  }
  return arr;
}

template <typename P, size_t LENGTH>
constexpr std::array<P, LENGTH> random_int_array(size_t low, size_t high) {
  std::array<P, LENGTH> arr{};
  uint16_t s = 0; // seed
  for (uint32_t i = 0; i < LENGTH; i++) {
    s = next_seed(s);
    arr[i] = gen_int(s, low, high);
  }
  return arr;
}

}; // namespace const_rand
