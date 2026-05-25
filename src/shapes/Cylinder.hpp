#pragma once

#include <vector>

#include "../ParticleShape.hpp"
#include "../rng.hpp"

class ScatteringPoint;

class Cylinder : public ParticleShape {
 public:
  double D = 1.;
  double L = 1.;
  double R = 0.5;
  double L2 = 0.5;

  Cylinder() = default;

  Cylinder(double diameter, double length)
      : D(diameter), L(length), R(0.5 * diameter), L2(0.5 * length) {}

  void readFromInput(std::istream& input) override {
    input >> D >> L;
    R = 0.5 * D;
    L2 = 0.5 * L;
  }

  void printShape() override {
    std::cout << "Cylinder: D = " << D << ", R = " << R << ", L = " << L
              << ", L2 = " << L2 << "\n";
  }

  double computeVolume() override { return M_PI * R * R * L; }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

std::vector<ScatteringPoint> Cylinder::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  auto Npoints = static_cast<size_t>(rhoSP * volume);

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    while (true) {
      sp.cm = {rng().randomDouble(-R, R), rng().randomDouble(-R, R),
               rng().randomDouble(-L2, L2)};

      if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1]) <= R) break;
    };
  }

  return sPoints;
}