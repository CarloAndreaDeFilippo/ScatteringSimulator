#pragma once

#include <vector>

#include "../ParticleShape.hpp"
#include "../rng.hpp"

class ScatteringPoint;

class Ellipsoid : public ParticleShape {
 public:
  std::array<double, 3> semi_axes = {{0.5, 0.5, 0.5}};

  Ellipsoid() = default;

  Ellipsoid(double a, double b, double c) : semi_axes{{a, b, c}} {}

  void readFromInput(std::istream& input) override {
    input >> semi_axes[0] >> semi_axes[1] >> semi_axes[2];
  }

  void printShape() override {
    std::cout << "Ellipsoid: a = " << semi_axes[0] << ", b = " << semi_axes[1]
              << ", c = " << semi_axes[2] << "\n";
  }

  double computeVolume() override {
    return (4. * M_PI / 3.) * semi_axes[0] * semi_axes[1] * semi_axes[2];
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

std::vector<ScatteringPoint> Ellipsoid::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  auto Npoints = static_cast<size_t>(rhoSP * volume);

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    while (true) {
      sp.cm = {rng().randomDouble(-semi_axes[0], semi_axes[0]),
               rng().randomDouble(-semi_axes[1], semi_axes[1]),
               rng().randomDouble(-semi_axes[2], semi_axes[2])};

      double distance = 0.;

      for (int ax = 0; ax < 3; ax++)
        distance += sp.cm[ax] * sp.cm[ax] / (semi_axes[ax] * semi_axes[ax]);

      if (distance <= 1) break;
    };
  }

  return sPoints;
}