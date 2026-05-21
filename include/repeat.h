#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

template <uint32_t COUNT, uint32_t STEP = 1, uint32_t START = 0, typename F>
constexpr void repeat(const F &func) {
  if constexpr (START < COUNT) {
    func(std::integral_constant<uint32_t, START>{});
    repeat<COUNT, STEP, START + STEP, F>(func);
  }
}
