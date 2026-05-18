#pragma once

#include <string>
#include <vector>

#include "FilesystemUtils.hpp"
#include "ScatteringPlane.hpp"
#include "ScatteringTypes.hpp"
#include "ScatteringVector.hpp"
#include "nlohmann/json.hpp"

class SimulationSettings {
 public:
  ScattType scattType = ScattType::Sq;

  std::vector<ScatteringVector> scattVectors;
  std::vector<ScatteringPlane> scattPlanes;
  double rhoSP = 1.;

  std::string configurationFolder;
  std::vector<std::string> configurationFiles;

  std::string outputFolder = "Data";
  std::string outputFolderRho1D = "Data/rho1D";
  std::string outputFolderRho2D = "Data/rho2D";

  bool saveCogli2 = false;
  std::string cogli2Folder = "Cogli2";

  SimulationSettings() = default;

  SimulationSettings(const std::string& settingsFile) {
    loadSettings(settingsFile);
  }

  void loadSettings(const std::string& settingsFile);
};