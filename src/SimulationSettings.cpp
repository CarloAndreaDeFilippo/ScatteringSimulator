#include "SimulationSettings.hpp"

#include <fstream>
#include <iostream>

#include "Utilities.hpp"

void SimulationSettings::loadSettings(const std::string& scattFile) {
  std::ifstream file(scattFile);
  if (!file.is_open()) {
    std::cout << "Error opening " << scattFile << "\n";
    std::exit(-1);
  }

  // Read simulation parameters from json file
  nlohmann::json settings;
  file >> settings;
  file.close();

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
    if (settings.contains("configurationsFolder")) {
      configurationFolder = settings["configurationsFolder"];
      configurationFiles = listFilesInDir(configurationFolder);
    }

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Output folder
  try {
    if (settings.contains("outputFolder")) {
      outputFolder = settings["outputFolder"];

      if (directoryExists(outputFolder) == false)
        makeDirectory(outputFolder);
    }

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Cogli1
  if (settings.contains("saveCogli2")) {
    saveCogli2 = settings["saveCogli2"];

    std::cout << settings["cogli2Folder"] << "\n";

    if (saveCogli2) {
      try {
        if (settings.contains("cogli2Folder")) {
          cogli2Folder = settings["cogli2Folder"];

          std::cout << cogli2Folder << "\n";

          if (directoryExists(cogli2Folder) == false)
            makeDirectory(cogli2Folder);
        }
      } catch (const std::exception& e) {
        std::cout << "Error parsing JSON: cogli2Folder not provided" << "\n";
        std::exit(-1);
      }
    }
  }
}