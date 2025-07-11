#pragma once

#include <array>
#include <complex>
#include <string>
#include <vector>

#include "ProgressBar.hpp"
#include "ScatteringPoint.hpp"

class Rho2D {
 public:
  std::array<double, 3> qmin = {{0., 0., 0.}};  // Minimum value of q components
  std::array<double, 3> dq = {{0.01, 0.01, 0.01}};
  std::array<size_t, 3> qqmax = {{100, 100, 100}};  // Maximum q index for matrices

  int axis1 = 0;
  int axis2 = 1;

  std::vector<std::vector<std::complex<double>>> pos_pos;  // q = (+qx, +qy)
  std::vector<std::vector<std::complex<double>>> pos_neg;  // q = (+qx, -qy)

  std::vector<std::vector<std::complex<double>>> neg_pos;  // q = (-qx, +qy)
  std::vector<std::vector<std::complex<double>>> neg_neg;  // q = (-qx, -qy)

  // void loadQ(const std::string infile);
  void initialize(const std::array<double, 3> dQ, const std::array<double, 3> Qmin, const std::array<size_t, 3> QQmax);
  // void setQ(const std::array<double, 3> Qmax, const std::array<double, 3> dQ);
  // void calculateRange(const size_t sp_in, const size_t sp_fin, const std::vector<ScatteringPoint>& scatteringPoints);
  // void calculateThreads(const std::vector<ScatteringPoint>& scatteringPoints, const int Nthreads);
  void calculateConjugates();
  void exportData(size_t NSP, std::string filename);
};