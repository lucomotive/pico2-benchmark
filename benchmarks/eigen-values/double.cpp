
#include "debug.h"
#include "run.h"

#include <cstdio>
#if defined(BUILD_PICO)
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include "pico/time.h"
#endif

int main() {
#if defined(BUILD_PICO)
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }
#endif

  // display
  debug<double>(4);
  debug<double>(5);

  // run
  run<double>();

  printf("EOF\n");
}
