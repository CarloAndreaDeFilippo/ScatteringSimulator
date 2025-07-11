#pragma once

#include <thread>
#include <vector>

#include "Particle.hpp"
#include "Rho1D.hpp"
#include "Rho2D.hpp"
#include "ScatteringPoint.hpp"

class ScatteringSystem {
 public:
  double rhoSP = 10.;                             // Density of scattering points
  size_t NSP = 0;                                 // Number of scattering points
  std::vector<ScatteringPoint> scatteringPoints;  // Total ensemble of scattering points

  bool Sq = false;  // Calculate S(q) (generate only one scattering point per particle)

  std::array<double, 3> qmin = {{0, 0, 0}};         // Minimum value of q components
  std::array<double, 3> qmax = {{1, 1, 1}};         // Maximum value of q components
  std::array<double, 3> dq = {{0.01, 0.01, 0.01}};  // Precision of q calculations
  std::array<size_t, 3> qqmax = {{100, 100, 100}};  // Maximum q index for matrices

  int axis1 = 0;
  int axis2 = 1;

  Rho1D rho1D_1, rho1D_2;  // FT of microscopic density along axis1 and axis2
  Rho2D rho2D;             // FT of microscopic density along the axis1/axis2 plane

  //* Functions
  void generateScatteringPoints(std::vector<Particle> particles);

  void loadQ(const std::string infile);

  void initializeRho();

  void calculateRho();

  void calculateRho1D();

  void cogli1(const std::array<double, 3>& L, const std::string filename, const bool append);
};