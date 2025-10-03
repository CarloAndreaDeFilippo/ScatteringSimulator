#include "ScatteringSimulation.hpp"

#include <array>
#include <fstream>
#include <iostream>

#include "Cogli2.hpp"
#include "ParticleSystem.hpp"
#include "ScatteringSystem.hpp"

void ScatteringSimulation::startSimulation() {
  std::cout << "#Total configurations: " << simSettings.configurationFiles.size() << "\n";

  // Loop over the configurations
  for (const auto& configuration : simSettings.configurationFiles) {
    std::string confName = configuration;
    std::cout << "Configuration " << confName << "\n";

    size_t lastindex = confName.find_last_of(".");
    std::string confNameNoExtension = confName.substr(0, lastindex);

    std::string particlesFile = simSettings.configurationFolder + "/" + confName;

    //  Load particles and initialize the system
    ParticleSystem partSys(particlesFile);

    ScatteringSystem scattSys(simSettings.scattType);

    scattSys.generateScatteringPoints(partSys.particles);

    if (simSettings.saveCogli2) {
      std::string outputCogli2 = simSettings.cogli2Folder + confNameNoExtension + ".mgl";
      cogli2::box(partSys.Lbox, outputCogli2);
      scattSys.cogli2(partSys.Lbox, outputCogli2, true);
    }

    // std::cout << "Number of scattering points: " << scattSys.NSP << "\n";

    // TODO: logscale support

    // TODO: check for finite-size effects in dq

    std::string rho1DFolderConfig = simSettings.outputFolderRho1D + confNameNoExtension + "/";
    std::string rho2DFolderConfig = simSettings.outputFolderRho2D + confNameNoExtension + "/";

    // Make subfolders for output data
    if (simSettings.scattVectors.size() > 0) {
      if (!directoryExists(rho1DFolderConfig))
        makeDirectory(rho1DFolderConfig);
    }

    if (simSettings.scattPlanes.size() > 0) {
      if (!directoryExists(rho2DFolderConfig))
        makeDirectory(rho2DFolderConfig);
    }

    // Initialize Rho1D
    for (auto& scattVec : simSettings.scattVectors)
      scattSys.vecRho1D.emplace_back(scattVec);

    // Initialize Rho2D
    for (auto& scattPlane : simSettings.scattPlanes)
      scattSys.vecRho2D.emplace_back(scattPlane);

    for (size_t vec = 0; vec < scattSys.vecRho1D.size(); ++vec) {
      auto& rho1d = scattSys.vecRho1D[vec];

      rho1d.calculateRho(scattSys.scatteringPoints);

      std::string outFile = rho1DFolderConfig + "axis_" + std::to_string(vec) + ".txt";
      rho1d.exportData(scattSys.NSP, outFile);
    }

    for (size_t pl = 0; pl < scattSys.vecRho2D.size(); ++pl) {
      auto& rho2d = scattSys.vecRho2D[pl];

      rho2d.calculateRho(scattSys.scatteringPoints);

      std::string outFile = rho2DFolderConfig + "plane_" + std::to_string(pl) + ".txt";
      rho2d.exportData(scattSys.NSP, outFile);
    }
  }
}
