

#include "hardware/regs/xip.h"
#include "hardware/structs/xip.h"
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <cstdio>
#include <cstdlib>

#include "rw_test.h"

#define PSRAM_SIZE_BYTES (8 * 1024 * 1024)
#define PSRAM_BASE_ADDRESS 0x11000000
// #define PSRAM_BASE_ADDRESS 0x15000000 // cached

#define PSRAM_CS_PIN 8

inline void psram_init() {
  gpio_set_function(PSRAM_CS_PIN, GPIO_FUNC_XIP_CS1); // CS for PSRAM
  xip_ctrl_hw->ctrl |= XIP_CTRL_WRITABLE_M1_BITS;
}

int main() {

  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected())
    sleep_ms(100);

  psram_init();

  constexpr uint32_t TEST_SIZE = 8 * 1024;

  // test psram
  printf("PSRAM Test ––––––––––\n");
  auto *psram = (uint32_t *)PSRAM_BASE_ADDRESS;
  auto psram_time = read_write_test(psram, TEST_SIZE);
  printf("Took: %llu µs\n", psram_time);
  printf("–––––––––––––––––––––\n");

  // test sram
  printf("RAM Test ––––––––––––\n");
  auto *buffer = new uint32_t[TEST_SIZE];
  auto sram_time = read_write_test(buffer, TEST_SIZE);
  printf("Took: %llu µs\n", sram_time);
  delete[] buffer;
  printf("–––––––––––––––––––––\n");

  sleep_ms(500);
}
