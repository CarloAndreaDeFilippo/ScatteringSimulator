#pragma once

#include <array>
#include <complex>
#include <string>
#include <vector>

#include "ProgressBar.hpp"
#include "ScatteringPlane.hpp"
#include "ScatteringPoint.hpp"

class Rho2D {
 public:
  ScatteringPlane qPlane;

  // NB: Given the symmetry of S(q) = rho(q) * rho(-q) we can calculate half of the (q1,q2) plane
  std::vector<std::vector<std::complex<double>>> pos_pos;  // q = (+q1, +q2)
  std::vector<std::vector<std::complex<double>>> pos_neg;  // q = (+q1, -q2)

  Rho2D() = default;

  Rho2D(const ScatteringPlane& scattPlane) {
    qPlane = scattPlane;
    pos_pos.resize(qPlane.q1Vector.qqmax);
    pos_neg.resize(qPlane.q1Vector.qqmax);

    for (auto& rho : pos_pos)
      rho.resize(qPlane.q2Vector.qqmax, 0.);

    for (auto& rho : pos_neg)
      rho.resize(qPlane.q2Vector.qqmax, 0.);
  }

  void calculateRho(const std::vector<ScatteringPoint>& scatteringPoints);
  void exportData(const size_t NSP, const std::string& filename);
};