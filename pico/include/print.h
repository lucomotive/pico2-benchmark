#pragma once

#include "Eigen/Dense"
#include <iostream>

using namespace Eigen;

// template <typename S> void print(const S &s, bool verbose) {
//   if (verbose) {
//     IOFormat fmt(FullPrecision, 0, ", ", "\n", "[", "]");

//     std::cout << "––––––––––––––––– TESTING ALGORITHM –––––––––––––––––"
//               << std::endl;
//     std::cout << "SOURCE = " << std::endl
//               << result.source.format(fmt) << std::endl;
//     std::cout << "Q = " << std::endl << result.Q.format(fmt) << std::endl;
//     std::cout << "R = " << std::endl << result.R.format(fmt) << std::endl;
//     auto approx = result.Q * result.R;
//     std::cout << "Q * R = " << std::endl
//               << (result.Q * result.R).format(fmt) << std::endl;
//     std::cout << "Error = " << (result.source - approx).norm() << std::endl;
//     std::cout << "TIME = " << result.time << "µs" << std::endl;
//     std::cout << "–––––––––––––––––––––––––––––––––––––––––––––––––––––"
//               << std::endl;
//   } else {
//     auto approx = result.Q * result.R;
//     printf("%u,%u,%llu,%e\n", result.source.rows(), result.source.cols(),
//            result.time, (result.source - approx).norm());
//   }
// }
