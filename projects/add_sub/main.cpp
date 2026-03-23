// set float or double
#define REAL float

#include "pico/stdlib.h"
#include "pico/time.h"
#include <iostream>

// Eigen
#include <Eigen/Dense>

// Matrizen global im bss-Speicher
Eigen::Matrix<REAL, 10, 10> A;

int main() {
  stdio_init_all();
  sleep_ms(5000);

  printf("Initialisiere Matrizen mit Random-Werten...\n");

  while (true) {
    A.setRandom();
    std::cout << "A = " << A << std::endl;
    sleep_ms(2000);
  }
}
