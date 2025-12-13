#pragma once

#include <array>
#include <complex>
#include <string>
#include <vector>

#include "ScatteringPoint.hpp"
#include "ScatteringVector.hpp"
#include "mathTools.hpp"

class Rho1D {
 public:
  ScatteringVector qVector;

  std::vector<std::complex<double>> rho;

  Rho1D() = default;

  Rho1D(const ScatteringVector& scattVec)
      : qVector(scattVec), rho(scattVec.qqmax, 0.0) {}

  void calculateRho(const std::vector<ScatteringPoint>& scatteringPoints);

  void computeRhoCPU(const std::vector<double>& projBase);
  void computeRhoCUDA(const std::vector<double>& projBase);

  void exportData(const size_t NSP, const std::string& filename);
};