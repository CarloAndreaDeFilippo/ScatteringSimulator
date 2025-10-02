#pragma once

#include <string>
#include <vector>

#include "ScatteringPlane.hpp"
#include "ScatteringTypes.hpp"
#include "ScatteringVector.hpp"
#include "Utilities.hpp"
#include "nlohmann/json.hpp"

class SimulationSettings {
 public:
  ScattType scattType = ScattType::Sq;

  std::vector<ScatteringVector> scattVectors;
  std::vector<ScatteringPlane> scattPlanes;
  double rhoSP;

  std::string configurationFolder;
  std::vector<std::string> configurationFiles;

  std::string outputFolder = "Data/rho1D/";

  bool saveCogli2 = false;
  std::string cogli2Folder = "Cogli2/";

  SimulationSettings() = default;

  SimulationSettings(const std::string& settingsFile) {
    loadSettings(settingsFile);
  }

  void loadSettings(const std::string& settingsFile);
};