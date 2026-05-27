
#pragma once

#include "benchmarks/utils.h"
#include <Eigen/Dense>
#include <iostream>

static const Eigen::IOFormat EIGEN_FMT(Eigen::FullPrecision, 0, ", ", "\n");

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

template <typename Mat> void print_float_matrix(const Mat &mat) {
  for (auto row : mat.rowwise()) {
    for (int i = 0; i < row.size(); i++) {
      printf("%.17g", row(i));
      if (i != row.size() - 1)
        printf(", ");
    }
    printf("\n");
  }
}
