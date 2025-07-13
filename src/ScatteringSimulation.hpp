#pragma once

#include <string>

#include "nlohmann/json.hpp"

class ScatteringSimulation {
 public:
  enum class SimType { OneDim,
                       TwoDim };

  SimType simType;
  std::vector<std::array<double, 3>> axes;
  double qmin;
  double qmax;
  double rhoSP;

  void loadSettings(const std::string& fileName);
};