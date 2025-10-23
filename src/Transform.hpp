#pragma once

#include <array>

class Transform {
 public:
  std::array<double, 3> cm = {{0., 0., 0.}};                     // Center of mass
  std::array<std::array<double, 3>, 3> R = {{{{1.0, 0.0, 0.0}},  // Orientation matrix
                                             {{0.0, 1.0, 0.0}},
                                             {{0.0, 0.0, 1.0}}}};
};