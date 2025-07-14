#pragma once

#include <string>

#include "ScatteringVector.hpp"
#include "Utilities.hpp"
#include "nlohmann/json.hpp"

class ScatteringSimulation {
 public:
  enum class SimType { OneDim,
                       TwoDim };

  enum class ScattType { Sq,
                         Iq };

  SimType simType = SimType::OneDim;
  ScattType scattType = ScattType::Sq;

  std::vector<ScatteringVector> scattVectors;
  double rhoSP;

  std::vector<std::string> configurationFiles;

  ScatteringSimulation() = default;

  ScatteringSimulation(const std::string& scattFile);

  void loadSettings(const std::string& scattFile);
};