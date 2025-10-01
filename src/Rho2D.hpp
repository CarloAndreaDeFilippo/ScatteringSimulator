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

  std::array<double, 3> qmin = {{0., 0., 0.}};  // Minimum value of q components
  std::array<double, 3> dq = {{0.01, 0.01, 0.01}};
  std::array<size_t, 3> qqmax = {{100, 100, 100}};  // Maximum q index for matrices

  int axis1 = 0;
  int axis2 = 1;

  std::vector<std::vector<std::complex<double>>> neg_pos;  // q = (-qx, +qy)
  std::vector<std::vector<std::complex<double>>> neg_neg;  // q = (-qx, -qy)

  void calculateConjugates();
  void exportData(size_t NSP, std::string filename);
};