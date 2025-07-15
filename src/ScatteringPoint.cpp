#include "ScatteringPoint.hpp"

std::string ScatteringPoint::cogli1(const std::array<double, 3>& Lbox) {
  double radius = 0.1;
  std::string color = "blue";

  std::string output = "";

  for (int ax = 0; ax < 3; ax++) {
    output += std::to_string(cm[ax] + 0.5 * Lbox[ax]) + " ";
  }

  output += "@ " + std::to_string(radius) + " C[" + color + "]\n";
  return output;
}