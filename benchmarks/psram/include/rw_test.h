#pragma once

#include "pico/time.h"
#include <cstdio>
#include <iostream>
#include <ostream>

int64_t read_write_test(uint32_t *buffer, uint32_t length) {
  const auto start = get_absolute_time();
  for (uint32_t i = 0; i < length; ++i)
    buffer[i] = i;

  bool success = true;
  uint32_t expected = 0;
  uint32_t got = 0;
  for (uint32_t i = 0; i < length; ++i) {
    if (buffer[i] != i) {
      success = false;
      expected = i;
      got = buffer[i];
      break;
    }
  }

  const auto stop = get_absolute_time();

  if (success)
    printf("Test finished\n");
  else
    std::cout << "Failed. expected: " << expected << " " << "got: " << got
              << std::endl;

  return absolute_time_diff_us(start, stop);
}

int64_t allocation_test() {}
