#pragma once

#include <string>
#include <vector>

#include "ScatteringTypes.hpp"
#include "ScatteringVector.hpp"
#include "SimulationTypes.hpp"
#include "Utilities.hpp"
#include "nlohmann/json.hpp"

class SimulationSettings {
 public:
  SimType simType = SimType::OneDim;
  ScattType scattType = ScattType::Sq;

  std::vector<ScatteringVector> scattVectors;
  double rhoSP;

  std::string configurationFolder;
  std::vector<std::string> configurationFiles;

  std::string rho1DFolder = "Data/rho1D/";

  SimulationSettings() = default;

  SimulationSettings(const std::string& scattFile) {
    loadSettings(scattFile);
  }

  void loadSettings(const std::string& scattFile);
};