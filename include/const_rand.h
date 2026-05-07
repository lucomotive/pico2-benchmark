#pragma once

#include "pico/rand.h"
#include "pico/time.h"
#include <array>

#include <Eigen/Dense>
#include <iostream>
#include <nlohmann/json.hpp>

namespace ConstRand {
using Seed = uint32_t;
constexpr uint32_t next_seed(Seed s) { return s * 1664525u + 1013904223u; }
constexpr double gen_float(Seed s) {
  const uint8_t SIZE = sizeof(Seed) * 8;
  return s * (1.0 / (double)(2 ^ SIZE));
}

// Generate a compile-time random array
template <typename P, uint32_t LENGTH>
constexpr std::array<P, LENGTH> random_const_array() {
  std::array<P, LENGTH> arr{};
  uint16_t s = 0; // seed
  for (uint32_t i = 0; i < LENGTH; i++) {
    s = next_seed(s);
    arr[i] = (P)gen_float(s);
  }
  return arr;
}

}; // namespace ConstRand
