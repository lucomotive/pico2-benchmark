#pragma once

#include "pico/stdio_usb.h"
#include "pico/time.h"

#include <pico/stdio.h>
#include <pico/types.h>

#include "debug.h"
#include "run.h"

using namespace benchmarks;

template <typename P> inline void bench_main() {
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  // display
  debug<4, P>();
  // run
  run<P>();

  stdio_deinit_all();
}
