
#include <cstdio>

#if defined(BUILD_PICO)
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include "pico/time.h"
#endif

#if defined(ENABLE_PSRAM)
#include "psram.h"
#endif

void setup() {
#if defined(BUILD_PICO)
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }
#endif

#if defined(ENABLE_PSRAM)
  const static size_t PSRAM_CS_PIN = 8;
  psram_setup(PSRAM_CS_PIN);
#endif
}
