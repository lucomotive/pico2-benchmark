#pragma once

#include "pico/rand.h"
#include <iostream>

class Benchmark {

public:
  virtual void run(bool debug) = 0;

  ~Benchmark() {}

  static uint32_t random_range_32(uint32_t low, uint32_t high) {
    return low + (get_rand_32() % (high - low + 1));
  }

  virtual void sof() = 0;
  void eof() { printf("EOF\n"); }
};
