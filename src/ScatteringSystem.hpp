#pragma once

#include <thread>
#include <vector>

#include "Particle.hpp"
#include "Rho1D.hpp"
#include "Rho2D.hpp"
#include "ScatteringPoint.hpp"
#include "ScatteringTypes.hpp"
#include "SimulationTypes.hpp"

class ScatteringSystem {
 public:
  double rhoSP = 1.;                              // Density of scattering points
  size_t NSP = 0;                                 // Number of scattering points
  std::vector<ScatteringPoint> scatteringPoints;  // Total ensemble of scattering points

  SimType simType = SimType::OneDim;
  ScattType scattType = ScattType::Sq;

  std::vector<Rho1D> vecRho1D;

  //! Placeholder for 2D scattering
  Rho2D rho2D;  // FT of microscopic density along the axis1/axis2 plane

  ScatteringSystem() = default;
  ScatteringSystem(ScattType sType) {
    scattType = sType;
  }

  //* Functions
  void generateScatteringPoints(const std::vector<Particle>& particles);
  void cogli1(const std::array<double, 3>& L, const std::string filename, const bool append);
};