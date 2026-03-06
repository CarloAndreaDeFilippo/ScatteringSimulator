#pragma once

#include <thread>
#include <vector>

#include "Particle.hpp"
#include "Rho1D.hpp"
#include "Rho2D.hpp"
#include "ScatteringPoint.hpp"
#include "ScatteringTypes.hpp"

class ScatteringSystem {
 public:
  ScattType scattType = ScattType::Sq;

  double rhoSP = 1.;  // Density of scattering points
  size_t NSP = 0;     // Number of scattering points
  std::vector<ScatteringPoint>
      scatteringPoints;  // Total ensemble of scattering points

  std::vector<Rho1D> vecRho1D;
  std::vector<Rho2D> vecRho2D;

  static constexpr size_t cogli2MaxSpheres = 100000;

  ScatteringSystem() = default;
  ScatteringSystem(ScattType sType, double rho = 1.0)
      : scattType(sType), rhoSP(rho) {}

  //* Functions
  void generateScatteringPoints(const std::vector<Particle>& particles,
                                const std::array<double, 3>& Lbox);
  void cogli2(const std::array<double, 3>& Lbox, const std::string& filename,
              const bool append);
};