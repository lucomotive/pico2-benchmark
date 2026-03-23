
#include "pico/stdlib.h"
#include <Eigen/Core>

#define REAL float

Eigen::Matrix<REAL, 10, 10> mat;

int main() {
  sleep_ms(5000); // Warten auf Serial-Verbindung

  println("Initialisiere Matrizen mit Random-Werten...\n");
  mat.setRandom();
  std::cout << "A = " << A << std::endl;
}
