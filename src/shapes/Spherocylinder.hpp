#pragma once

#include <vector>

#include "../ParticleShape.hpp"
#include "../rng.hpp"

class ScatteringPoint;

class Spherocylinder : public ParticleShape {
 public:
  double D = 1.;
  double L = 1.;
  double R = 0.5;
  double L2 = 0.5;

  Spherocylinder() = default;

  Spherocylinder(double diameter, double length)
      : D(diameter), L(length), R(0.5 * D), L2(0.5 * L) {}

  void readFromInput(std::istream& input) override {
    input >> D >> L;
    R = 0.5 * D;
    L2 = 0.5 * L;
  }

  void printShape() override {
    std::cout << "Spherocylinder: D = " << D << ", R = " << R << ", L = " << L
              << ", L2 = " << L2 << "\n";
  }

  double computeVolume() override {
    return (4. * M_PI / 3.) * R * R * R + M_PI * R * R * L;
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

std::vector<ScatteringPoint> Spherocylinder::generateScatteringPoints(
    double rhoSP) {
  double volume = computeVolume();

  auto Npoints = static_cast<size_t>(rhoSP * volume);

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    while (true) {
      sp.cm = {rng().randomDouble(-R, R), rng().randomDouble(-R, R),
               rng().randomDouble(-(R + L2), R + L2)};

      if (std::abs(sp.cm[2]) <= L2) {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1]) <= R) break;
      } else if (sp.cm[2] > L2) {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1] +
                 (sp.cm[2] - L2) * (sp.cm[2] - L2)) <= R)
          break;
      } else if (sp.cm[2] < L2) {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1] +
                 (sp.cm[2] + L2) * (sp.cm[2] + L2)) <= R)
          break;
      }

      /*else {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1] + sp.cm[2] * sp.cm[2]
      - sgn(sp.cm[2]) * L2 * L2) <= R) break;
      }*/
    };
  }

  return sPoints;
}