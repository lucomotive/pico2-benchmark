#include "pico/stdlib.h"
#include <iostream>


int main () {
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  printf("Welcome to pico2 eigen benchmark.");

  while (true) {
    printf("Choose a benchmark to run...");

    char buf[64];
    gets(&buf);

    printf("%s", buf);
  }




}
