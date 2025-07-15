#include "ScatteringSimulation.hpp"

#include <array>
#include <fstream>
#include <iostream>

#include "Cogli2.hpp"
#include "ScatteringSystem.hpp"

void ScatteringSimulation::startSimulation() {
  std::cout << "#Total configurations: " << simSettings.configurationFiles.size() << "\n";

  // Loop over the configurations
  for (auto& configuration : simSettings.configurationFiles) {
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

    std::cout << "Number of scattering points: " << scattSys.NSP << "\n";

    //! Only 1D sim for the moment

    // TODO: logscale support
    // TODO: 2D planes

    // TODO: check for finite-size effects in dq

    std::string outputFolder = simSettings.outputFolder + confNameNoExtension;
    if (directoryExists(outputFolder) == false)
      makeDirectory(outputFolder);

    // Initialize Rho1d
    for (auto& scattVec : simSettings.scattVectors)
      scattSys.vecRho1D.emplace_back(scattVec);

    for (size_t vec = 0; vec < scattSys.vecRho1D.size(); vec++) {
      auto& rho1d = scattSys.vecRho1D[vec];

      rho1d.qVector.qqmax = static_cast<int>((rho1d.qVector.qmax - rho1d.qVector.qmin) / rho1d.qVector.dq);

      rho1d.qVector.qValues.reserve(rho1d.qVector.qqmax);

      for (size_t qq = 0; qq < rho1d.qVector.qqmax; qq++) {
        double q = rho1d.qVector.qmin + qq * rho1d.qVector.dq;

        rho1d.qVector.qValues.push_back(q);
      }

      rho1d.rho.resize(rho1d.qVector.qqmax, 0.);

      rho1d.calculateRho(scattSys.scatteringPoints);

      std::string outFile = outputFolder + "/axis_" + std::to_string(vec) + ".txt";
      rho1d.exportData(scattSys.NSP, outFile);
    }
  }
}
