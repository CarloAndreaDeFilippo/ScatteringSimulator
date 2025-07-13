#pragma once

#include <string>

#include "nlohmann/json.hpp"

class ScatteringSimulation {
 public:
  enum class SimType { OneDim,
                       TwoDim };

  enum class ScattType { Sq,
                         Iq };

  SimType simType = SimType::OneDim;
  ScattType scattType = ScattType::Sq;

  std::vector<std::array<double, 3>> axes;
  double qmin;
  double qmax;
  double rhoSP;

  ScatteringSimulation() = default;

  ScatteringSimulation(const std::string& scattFile);

  void loadSettings(const std::string& scattFile);
};