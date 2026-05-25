

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

  const size_t a_size = 16;
  const size_t b_size = 3;
  const size_t c_size = 9;

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

  const size_t d_size = 8;
  const size_t e_size = 1;
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

  printf("DEALLOCATING ––––––––\n");
  psram::ps_free(c);
  psram::ps_free(d);
  psram::ps_free(e);
  printf("–––––––––––––––––––––\n");

  printf("PROBING –––––––––––––\n");
  psram::probe(true);
  printf("–––––––––––––––––––––\n");

  printf("ALLOCATE HUGE –––––––\n");
  const size_t f_size = 1024 * 1024 * 2;
  auto [f, time_f] = allocate<DATA_T>(f_size);
  std::fill(f, f + f_size, 12);
  printf("Ptr: %p, took: %llu µs\n", f, time_f);

  const size_t g_size = 1024 * 1024 * 3;
  auto [g, time_g] = allocate<DATA_T>(g_size);
  std::fill(g, g + g_size, 7);
  printf("Ptr: %p, took: %llu µs\n", g, time_g);

  const size_t h_size = 1024 * 1024 * 6;
  auto [h, time_h] = allocate<DATA_T>(h_size);
  if (h)
    std::fill(h, h + h_size, 9);
  printf("Ptr: %p, took: %llu µs\n", h, time_h);
  printf("–––––––––––––––––––––\n");

  printf("PROBING –––––––––––––\n");
  psram::probe(true, 128);
  printf("–––––––––––––––––––––\n");

  sleep_ms(500);
}
