#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "nlohmann/json.hpp"

class ScatteringVector {
 public:
  double dq = 0.01;    // Step size in q
  double qmin = 0.;    // Minimum q
  double qmax = 1.;    // Maximum q
  size_t qqmax = 100;  // Maximum q index

  std::array<double, 3> qAxis = {1., 0., 0.};  // Direction of the q vector
  std::vector<double> qValues;                 // Vector of q values

  ScatteringVector() = default;
  explicit ScatteringVector(const nlohmann::json& j);  // Constructor from json
  void calculateQValues();
};