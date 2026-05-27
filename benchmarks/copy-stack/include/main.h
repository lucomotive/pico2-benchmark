
#include "debug.h"
#include "run.h"

#include <cstdio>

#include "setup.h"

template <typename P> void bench_main() {
  setup();

  // display
  debug<P, 4>();
  // loop
  run<P>();

  printf("EOF\n");
  sleep_ms(500);
}
