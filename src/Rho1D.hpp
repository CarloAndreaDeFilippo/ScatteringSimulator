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

  Rho1D(const ScatteringVector& scattVec) {
    qVector = scattVec;
  }

  void calculateRho(const std::vector<ScatteringPoint>& scatteringPoints);
  void exportData(const size_t NSP, const std::string& filename);
};