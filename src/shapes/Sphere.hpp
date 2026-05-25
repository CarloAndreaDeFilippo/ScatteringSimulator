#pragma once

#include <cmath>
#include <vector>

#include "../ParticleShape.hpp"
#include "../mathTools.hpp"
#include "../rng.hpp"

class ScatteringPoint;

class Sphere : public ParticleShape {
 public:
  double D = 1.;
  double R = 0.5;

  Sphere() = default;
  Sphere(double diameter) : D(diameter), R(0.5 * diameter) {}

  void readFromInput(std::istream& input) override {
    input >> D;
    R = 0.5 * D;
  }

  void printShape() override {
    std::cout << "Sphere: D = " << D << ", R = " << R << "\n";
  }

  double computeVolume() override { return (4. * M_PI / 3.) * R * R * R; }
  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

std::vector<ScatteringPoint> Sphere::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  auto Npoints = static_cast<size_t>(rhoSP * volume);

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    while (true) {
      sp.cm = {rng().randomDouble(-R, R), rng().randomDouble(-R, R),
               rng().randomDouble(-R, R)};

      if (vectorNorm(sp.cm) <= R) break;
    };
  }

  return sPoints;
}