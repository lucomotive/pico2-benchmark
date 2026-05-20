
#pragma once

#include <Eigen/Dense>
#include <iostream>

static const Eigen::IOFormat EIGEN_FMT(Eigen::FullPrecision, 0, ", ", "\n", "[",
                                       "]");

// a function that takes an arbitrary amount of parameters and prints them with
// a separator SEP
// values must be formattable by std::cout
template <char SEP = ',', typename T, typename... V>
void print_all(T val, V... vars) {
  std::cout << val;
  if constexpr (sizeof...(vars) != 0) {
    std::cout << SEP;
    print_all<SEP>(vars...);
    return;
  }

  std::cout << std::endl;
}
