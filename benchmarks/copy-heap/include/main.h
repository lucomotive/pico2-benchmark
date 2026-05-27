
#include "debug.h"
#include "run.h"

#include <cstdio>

#include "setup.h"

template <typename P> void banch_main() {
  setup();

  // display
  debug<P>(4);
  // run
  run<P>();

  printf("EOF\n");
}
