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

    size_t lastindex = confName.find_last_of('.');
    std::string confNameNoExtension = confName.substr(0, lastindex);

    std::string particlesFile = simSettings.configurationFolder + confName;

    //  Load particles and initialize the system
    ParticleSystem partSys(particlesFile);

    ScatteringSystem scattSys(simSettings.scattType, simSettings.rhoSP);

    scattSys.generateScatteringPoints(partSys.particles);

    if (simSettings.saveCogli2 == true) {
      std::string outputCogli2 = simSettings.cogli2Folder + confNameNoExtension + ".mgl";
      cogli2::box(partSys.Lbox, outputCogli2);
      scattSys.cogli2(partSys.Lbox, outputCogli2, true);
    }

    // TODO: logscale support

    // TODO: check for finite-size effects in dq

    // Rho1D

    std::string rho1DFolderConfig = simSettings.outputFolderRho1D + confNameNoExtension + "/";

    if (simSettings.scattVectors.size() > 0) {
      if (!directoryExists(rho1DFolderConfig))
        makeDirectoryRecursive(rho1DFolderConfig);
    }

    // Initialize Rho1D
    for (auto& scattVec : simSettings.scattVectors)
      scattSys.vecRho1D.emplace_back(scattVec);

    for (size_t vec = 0; vec < scattSys.vecRho1D.size(); ++vec) {
      auto& rho1d = scattSys.vecRho1D[vec];

      rho1d.calculateRho(scattSys.scatteringPoints);

      std::string outFile = rho1DFolderConfig + "axis_" + std::to_string(vec) + ".txt";
      rho1d.exportData(scattSys.NSP, outFile);
    }

    // Rho2D

    std::string rho2DFolderConfig = simSettings.outputFolderRho2D + confNameNoExtension + "/";

    if (simSettings.scattPlanes.size() > 0) {
      if (!directoryExists(rho2DFolderConfig))
        makeDirectoryRecursive(rho2DFolderConfig);
    }

    // Initialize Rho2D
    for (auto& scattPlane : simSettings.scattPlanes)
      scattSys.vecRho2D.emplace_back(scattPlane);

    for (size_t pl = 0; pl < scattSys.vecRho2D.size(); ++pl) {
      auto& rho2d = scattSys.vecRho2D[pl];

      rho2d.calculateRho(scattSys.scatteringPoints);

      std::string outFile = rho2DFolderConfig + "plane_" + std::to_string(pl) + ".txt";
      rho2d.exportData(scattSys.NSP, outFile);
    }
  }
}
