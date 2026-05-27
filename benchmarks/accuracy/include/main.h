#include "setup.h"

#include "debug.h"
#include "run.h"

template <typename P> void bench_main() {
  setup();

  // display
  debug<P>(3);
  debug<P>(4);

  // run
  run<P>();

  printf("EOF\n");
  sleep_ms(500);
}
