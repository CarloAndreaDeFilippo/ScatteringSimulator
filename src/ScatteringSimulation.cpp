#include "ScatteringSimulation.hpp"

#include <array>
#include <fstream>
#include <iostream>

void ScatteringSimulation::loadConfig(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cout << "Error opening " << fileName << "\n";
    std::exit(-1);
  }

  // Read simulation parameters from json file
  nlohmann::json config;
  file >> config;

  std::cout << config << "\n";

  // Simulation type
  try {
    std::string typeStr = config.at("simType");
    if (typeStr == "OneDim" || typeStr == "1D") {
      simType = SimType::OneDim;
    } else {
      throw std::runtime_error("Unknown simType: " + typeStr);
    }
  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  try {
    if (config.contains("axes")) {
      for (const auto& axis : config["axes"]) {
        axes.push_back({axis[0], axis[1], axis[2]});
      }
    }

    qmin = config.value("qmin", 0.0);
    qmax = config.value("qmax", 0.0);
    rhoSP = config.value("rhoSP", 0.0);

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  file.close();
}