#pragma once

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

class ScatteringPoint {
 public:
  std::array<double, 3> cm = {{0., 0., 0.}};

  std::string cogli1(const std::array<double, 3>& L);
};