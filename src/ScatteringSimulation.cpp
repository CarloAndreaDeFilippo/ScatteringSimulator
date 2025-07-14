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

  // Scattering vectors
  try {
    if (settings.contains("scattVectors")) {
      for (const auto& vector : settings["scattVectors"]) {
        ScatteringVector scattVec;

        scattVec.qAxis = {vector.at("direction").at(0).get<double>(),
                          vector.at("direction").at(1).get<double>(),
                          vector.at("direction").at(2).get<double>()};

        if (vector.contains("qmin"))
          scattVec.qmin = vector.at("qmin").get<double>();

        if (vector.contains("qmax"))
          scattVec.qmax = vector.at("qmax").get<double>();

        if (vector.contains("dq"))
          scattVec.dq = vector.at("dq").get<double>();

        scattVectors.push_back(scattVec);
      }
    }

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Mesh density
  if (settings.contains("rhoSP"))
    rhoSP = settings.at("rhoSP").get<double>();

  // Configurations folder and files
  try {
    if (settings.contains("ConfigurationsFolder"))
      configurationFiles = listFilesInDir(settings["ConfigurationsFolder"]);
  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  file.close();
}