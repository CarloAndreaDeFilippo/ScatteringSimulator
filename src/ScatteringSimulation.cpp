#include "ScatteringSimulation.hpp"

#include <array>
#include <fstream>
#include <iostream>

ScatteringSimulation::ScatteringSimulation(const std::string& scattFile) {
  loadSettings(scattFile);
}

void ScatteringSimulation::loadSettings(const std::string& scattFile) {
  std::ifstream file(scattFile);
  if (!file.is_open()) {
    std::cout << "Error opening " << scattFile << "\n";
    std::exit(-1);
  }

  // Read simulation parameters from json file
  nlohmann::json settings;
  file >> settings;

  std::cout << settings << "\n";

  // Simulation type
  try {
    std::string typeStr = settings.at("simType");
    if (typeStr == "1D") {
      simType = SimType::OneDim;
    } else {
      throw std::runtime_error("Unknown simType: " + typeStr);
    }
  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Scattering type
  try {
    std::string typeStr = settings.at("scattType");
    if (typeStr == "Sq") {
      scattType = ScattType::Sq;
    } else if (typeStr == "Iq") {
      scattType = ScattType::Iq;
    } else {
      throw std::runtime_error("Unknown scattType: " + typeStr);
    }
  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  try {
    if (settings.contains("axes")) {
      for (const auto& axis : settings["axes"]) {
        axes.push_back({axis[0], axis[1], axis[2]});
      }
    }

    qmin = settings.value("qmin", 0.0);
    qmax = settings.value("qmax", 0.0);
    rhoSP = settings.value("rhoSP", 0.0);

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  file.close();
}