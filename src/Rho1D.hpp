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
  double dq = 0.01;
  double qmin = 0.;    // Minimum q
  size_t qqmax = 100;  // Maximum q index

  std::array<double, 3> qAxis = {1., 0., 0.};
  std::vector<double> qValues;

  std::vector<std::complex<double>> rho;

  Rho1D() = default;

  Rho1D(const ScatteringVector& scattVec) {
    qVector = scattVec;
  }

  Rho1D(std::array<double, 3> qAx, std::vector<double> qVals) {
    qAxis = qAx;
    qValues = qVals;

    rho.resize(qValues.size());
  }

  void initialize(const double dQ, const double Qmin, const size_t QQmax);
  void calculateRho(const std::vector<ScatteringPoint>& scatteringPoints);

  void exportData(const size_t NSP, const std::string filename);
};