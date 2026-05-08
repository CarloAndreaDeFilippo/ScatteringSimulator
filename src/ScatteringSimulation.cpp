#include "ScatteringSimulation.hpp"

#include <array>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

#include "Cogli2.hpp"
#include "ParticleSystem.hpp"
#include "ScatteringSystem.hpp"

#ifdef USE_CUDA
#include "cuda/GPUMemoryManager.hpp"
#endif

namespace fs = boost::filesystem;

void ScatteringSimulation::startSimulation() {
  std::cout << "#Total configurations: "
            << simSettings.configurationFiles.size() << "\n";

  // Loop over the configurations
  for (const auto& configuration : simSettings.configurationFiles) {
    fs::path confName(configuration);
    std::cout << "Configuration " << confName.string() << "\n";

    std::string confNameNoExtension = confName.stem().string();

    fs::path particlesFilePath =
        fs::path(simSettings.configurationFolder) / confName.filename();
    std::string particlesFile = particlesFilePath.string();

    //  Load particles and initialize the system
    ParticleSystem partSys(particlesFile);

    ScatteringSystem scattSys(simSettings.scattType, simSettings.rhoSP);

    scattSys.generateScatteringPoints(partSys.particles, partSys.Lbox);

    if (simSettings.saveCogli2 == true) {
      fs::path outputCogli2Path =
          fs::path(simSettings.cogli2Folder) / (confNameNoExtension + ".mgl");
      std::string outputCogli2 = outputCogli2Path.string();
      cogli2::box(partSys.Lbox, outputCogli2);
      scattSys.cogli2(partSys.Lbox, outputCogli2, true);
    }

#ifdef USE_CUDA
    // Uploading scattering points to GPU
    GPUMemoryManager gpuMemory;

    gpuMemory.uploadScatteringPoints(scattSys.scatteringPoints);
    std::cout << "Uploaded " << gpuMemory.getNumScatteringPoints()
              << " scattering points to GPU" << std::endl;
#endif

    // TODO: logscale support

    // TODO: spherical average S(q) or I(q)

    // TODO: check for finite-size effects in dq

    // Rho1D

    fs::path rho1DFolderPath =
        fs::path(simSettings.outputFolderRho1D) / confNameNoExtension;

    if (!simSettings.scattVectors.empty()) {
      if (!fs::exists(rho1DFolderPath)) fs::create_directories(rho1DFolderPath);
    }

    // Initialize Rho1D
    for (auto& scattVec : simSettings.scattVectors)
      scattSys.vecRho1D.emplace_back(scattVec);

    for (size_t vec = 0; vec < scattSys.vecRho1D.size(); ++vec) {
      auto& rho1d = scattSys.vecRho1D[vec];

#ifdef USE_CUDA
      rho1d.calculateRhoGPU(gpuMemory);  // Pass GPU memory manager
#else
      rho1d.calculateRhoCPU(scattSys.scatteringPoints);
#endif

      fs::path outFilePath =
          rho1DFolderPath / ("axis_" + std::to_string(vec) + ".txt");
      rho1d.exportData(scattSys.NSP, outFilePath.string());
    }

    // Rho2D

    fs::path rho2DFolderPath =
        fs::path(simSettings.outputFolderRho2D) / confNameNoExtension;

    if (!simSettings.scattPlanes.empty()) {
      if (!fs::exists(rho2DFolderPath)) fs::create_directories(rho2DFolderPath);
    }

    // Initialize Rho2D
    for (auto& scattPlane : simSettings.scattPlanes)
      scattSys.vecRho2D.emplace_back(scattPlane);

    for (size_t pl = 0; pl < scattSys.vecRho2D.size(); ++pl) {
      auto& rho2d = scattSys.vecRho2D[pl];

#ifdef USE_CUDA
      rho2d.calculateRhoGPU(gpuMemory);  // Pass GPU memory manager
#else
      rho2d.calculateRhoCPU(scattSys.scatteringPoints);
#endif

      fs::path outFilePath =
          rho2DFolderPath / ("plane_" + std::to_string(pl) + ".txt");
      rho2d.exportData(scattSys.NSP, outFilePath.string());
    }
  }
}
