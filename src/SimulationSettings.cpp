#include "SimulationSettings.hpp"

#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

#include "Utilities.hpp"
#include "mathTools.hpp"

namespace fs = boost::filesystem;

void SimulationSettings::loadSettings(const std::string& settingsFile) {
  std::ifstream file(settingsFile);
  if (!file.is_open()) {
    std::cout << "Error opening " << settingsFile << "\n";
    std::exit(-1);
  }

  // Read simulation parameters from json file
  nlohmann::json settings;
  file >> settings;
  file.close();

  // Print chosen settings
  for (auto it = settings.begin(); it != settings.end(); ++it) {
    std::cout << it.key() << ": " << it.value() << '\n';
  }

  // Scattering type (Sq or Iq)
  try {
    std::string parseScattType = settings.at("scattType");
    if (parseScattType == "Sq") {
      scattType = ScattType::Sq;
    } else if (parseScattType == "Iq") {
      scattType = ScattType::Iq;
    } else {
      throw std::runtime_error("Unknown scattType: " + parseScattType);
    }
  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Check if no ScatteringVector or ScatteringPlane is defined
  if (!settings.contains("scattVectors") && !settings.contains("scattPlanes")) {
    std::cout << "No scattering vectors or planes provided";
    std::exit(-1);
  }

  // Scattering vectors (1D rho calculation)
  for (const auto& vector : settings["scattVectors"]) {
    scattVectors.emplace_back(vector);
  }

  // Scattering planes (2D rho calculation)
  for (const auto& plane : settings["scattPlanes"]) {
    scattPlanes.emplace_back(plane);
  }

  // Mesh density
  if (settings.contains("rhoSP")) rhoSP = settings.at("rhoSP").get<double>();

  // Configurations folder and files
  try {
    if (!settings.contains("configurationsFolder")) {
      throw std::runtime_error("No configurationsFolder provided");
    }

    configurationFolder = settings["configurationsFolder"];
    configurationFiles = listFilesInDir(configurationFolder);

    if (configurationFiles.size() == 0) {
      throw std::runtime_error("No configuration files found in " +
                               configurationFolder);
    }

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Output folders

  if (settings.contains("outputFolder")) {
    outputFolder = settings["outputFolder"];
  }

  if (!directoryExists(outputFolder)) makeDirectory(outputFolder);

  if (settings.contains("scattVectors")) {
    outputFolderRho1D = (fs::path(outputFolder) / "rho1D").string();
    if (!directoryExists(outputFolderRho1D)) makeDirectory(outputFolderRho1D);
  }

  if (settings.contains("scattPlanes")) {
    outputFolderRho2D = (fs::path(outputFolder) / "rho2D").string();
    if (!directoryExists(outputFolderRho2D)) makeDirectory(outputFolderRho2D);
  }

  // Cogli2 output
  if (settings.contains("saveCogli2") &&
      settings["saveCogli2"].get<bool>() == true) {
    saveCogli2 = true;

    if (settings.contains("cogli2Folder"))
      cogli2Folder = settings["cogli2Folder"];

    if (!directoryExists(cogli2Folder)) makeDirectory(cogli2Folder);
  }
}