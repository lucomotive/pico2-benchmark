

#include "allocate.h"
#include "deallocate.h"

#include "my_rand.h"
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <algorithm>
#include <cstdio>
#include <vector>

#include "ps_ram.h"

#define PSRAM_CS_PIN 8

int main() {

  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected())
    sleep_ms(100);

  psram::init(PSRAM_CS_PIN);
  psram::init_heap();

  using DATA_T = uint8_t;

  const size_t a_size = 5;
  const size_t b_size = 3;
  const size_t c_size = 7;

  const size_t d_size = 7;
  const size_t e_size = 1;

  printf("ALLOCATING ––––––––––\n");
  auto [a, time_a] = allocate<DATA_T>(a_size);
  std::fill(a, a + a_size, 1);
  printf("Ptr: %p, took: %llu µs\n", a, time_a);

  auto [b, time_b] = allocate<DATA_T>(b_size);
  std::fill(b, b + b_size, 2);
  printf("Ptr: %p, took: %llu µs\n", b, time_b);

  auto [c, time_c] = allocate<DATA_T>(c_size);
  std::fill(c, c + c_size, 3);
  printf("Ptr: %p, took: %llu µs\n", c, time_c);
  printf("–––––––––––––––––––––\n");

  printf("PROBING –––––––––––––\n");
  psram::probe(true);
  printf("–––––––––––––––––––––\n");

  printf("DEALLOCATING ––––––––\n");
  psram::ps_free(b);
  psram::ps_free(a);
  printf("–––––––––––––––––––––\n");

  printf("PROBING –––––––––––––\n");
  psram::probe(true);
  printf("–––––––––––––––––––––\n");

  printf("ALLOCATING ––––––––––\n");
  auto [d, time_d] = allocate<DATA_T>(d_size);
  std::fill(d, d + d_size, 4);
  printf("Ptr: %p, took: %llu µs\n", d, time_d);
  auto [e, time_e] = allocate<DATA_T>(e_size);
  std::fill(e, e + e_size, 5);
  printf("Ptr: %p, took: %llu µs\n", e, time_e);
  printf("–––––––––––––––––––––\n");

  printf("PROBING –––––––––––––\n");
  psram::probe(true);
  printf("–––––––––––––––––––––\n");

  sleep_ms(500);
}
