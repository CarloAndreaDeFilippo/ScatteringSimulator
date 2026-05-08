#pragma once

#include <array>
#include <complex>
#include <string>
#include <vector>

#include "ScatteringPoint.hpp"
#include "ScatteringVector.hpp"
#include "mathTools.hpp"

#ifdef USE_CUDA
class GPUMemoryManager;
#endif

class Rho1D {
 public:
  ScatteringVector qVector;

  std::vector<std::complex<double>> rho;

  Rho1D() = default;

  Rho1D(const ScatteringVector& scattVec)
      : qVector(scattVec), rho(scattVec.qqmax, 0.0) {}

  void calculateRhoCPU(const std::vector<ScatteringPoint>& scatteringPoints);

#ifdef USE_CUDA
  void calculateRhoGPU(const GPUMemoryManager& gpuMemory);

#endif

  void exportData(const size_t NSP, const std::string& filename);
};