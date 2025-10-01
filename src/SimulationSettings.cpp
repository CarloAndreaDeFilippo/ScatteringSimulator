#include "SimulationSettings.hpp"

#include <fstream>
#include <iostream>

#include "Utilities.hpp"
#include "mathTools.hpp"

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

  // Simulation type
  try {
    std::string parseSimType = settings.at("simType");
    if (parseSimType == "1D") {
      simType = SimType::OneDim;
    } else {
      throw std::runtime_error("Unknown simType: " + parseSimType);
    }
  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
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

  // Scattering vectors
  try {
    if (!settings.contains("scattVectors")) {
      throw std::runtime_error("No scattering vectors provided");
    }
    for (const auto& vector : settings["scattVectors"]) {
      ScatteringVector scattVec;

      scattVec.qAxis = {vector.at("direction").at(0).get<double>(),
                        vector.at("direction").at(1).get<double>(),
                        vector.at("direction").at(2).get<double>()};

      // Normalization of the scattering vector
      normalizeVector(scattVec.qAxis);

      if (vector.contains("qmin"))
        scattVec.qmin = vector.at("qmin").get<double>();

      if (vector.contains("qmax"))
        scattVec.qmax = vector.at("qmax").get<double>();

      if (vector.contains("dq"))
        scattVec.dq = vector.at("dq").get<double>();

      scattVec.qqmax = static_cast<int>((scattVec.qmax - scattVec.qmin) / scattVec.dq);

      scattVec.qValues.reserve(scattVec.qqmax);

      for (size_t qq = 0; qq < scattVec.qqmax; ++qq) {
        double q = scattVec.qmin + qq * scattVec.dq;

        scattVec.qValues.push_back(q);
      }

      scattVectors.push_back(scattVec);
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
    if (!settings.contains("configurationsFolder")) {
      throw std::runtime_error("No configurationsFolder provided");
    }

    configurationFolder = settings["configurationsFolder"];
    configurationFiles = listFilesInDir(configurationFolder);

    if (configurationFiles.size() == 0) {
      throw std::runtime_error("No configuration files found in " + configurationFolder);
    }

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Output folder
  try {
    if (!settings.contains("outputFolder")) {
      throw std::runtime_error("No outputFolder provided");
    }
    outputFolder = settings["outputFolder"];

    if (!directoryExists(outputFolder))
      makeDirectory(outputFolder);

  } catch (const std::exception& e) {
    std::cout << "Error parsing JSON: " << e.what() << "\n";
    std::exit(-1);
  }

  // Cogli2 output
  if (settings.contains("saveCogli2") && settings["saveCogli2"] == true) {
    saveCogli2 = true;

    try {
      if (!settings.contains("cogli2Folder")) {
        throw std::runtime_error("No cogli2Folder provided");
      }
      cogli2Folder = settings["cogli2Folder"];

      if (directoryExists(cogli2Folder) == false)
        makeDirectory(cogli2Folder);

    } catch (const std::exception& e) {
      std::cout << "Error parsing JSON: cogli2Folder not provided" << "\n";
      std::exit(-1);
    }
  }
}