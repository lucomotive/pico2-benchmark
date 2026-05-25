#pragma once

#include "debug.h"
#include "run.h"

#include <cstdio>
#if defined(BUILD_PICO)
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include "pico/time.h"
#endif
#include "ps_ram.h"

#if defined(ENABLE_PSRAM)
// void *operator new(size_t size) {
//   void *ptr = malloc(size);
//   if (ptr)
//     return ptr;
//   ptr = psram::ps_malloc(size);
//   if (ptr)
//     return ptr;
//   throw std::bad_alloc();
// }

void *operator new(size_t size, std::nothrow_t) noexcept {
  void *ptr = malloc(size);
  if (ptr)
    return ptr;
  return psram::ps_malloc(size);
}

void operator delete(void *ptr) noexcept {
  if (!ptr)
    return;
  if (psram::is_psram(ptr))
    psram::ps_free(ptr);
  else
    free(ptr);
}

void *operator new[](size_t size) { return ::operator new(size); }

void operator delete[](void *ptr) noexcept { ::operator delete(ptr); }
#endif

template <typename P> void banch_main() {
#if defined(BUILD_PICO)
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }
#endif

#if defined(ENABLE_PSRAM)
  const static size_t PSRAM_CS_PIN = 8;
  psram::init(PSRAM_CS_PIN);
  psram::init_heap();
#endif

  // display
  debug<P>(4, 4, 4);
  debug<P>(2, 4, 3);

  // run
  run<P>();

  printf("EOF\n");
}
