#pragma once

#include <vector>

#include "../ParticleShape.hpp"
#include "../rng.hpp"

class ScatteringPoint;

class Superquadric : public ParticleShape {
 public:
  std::array<double, 3> semi_axes = {{0.5, 0.5, 0.5}};
  std::array<double, 3> exponents = {{2, 2, 2}};

  Superquadric() = default;

  Superquadric(double a, double b, double c, double r, double s, double t)
      : semi_axes{{a, b, c}}, exponents{{r, s, t}} {}

  void readFromInput(std::istream& input) override {
    input >> semi_axes[0] >> semi_axes[1] >> semi_axes[2];
    input >> exponents[0] >> exponents[1] >> exponents[2];
  }

  void printShape() override {
    std::cout << "Superquadric:\n";
    std::cout << "Semiaxes: A = " << semi_axes[0] << ", B = " << semi_axes[1]
              << ", C = " << semi_axes[2] << "\n";
    std::cout << "Exponents: r = " << exponents[0] << ", s = " << exponents[1]
              << ", t = " << exponents[2] << "\n";
  }

  //! Volume of the box containing the particle
  double computeVolume() override {
    return 8. * semi_axes[0] * semi_axes[1] * semi_axes[2];
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

std::vector<ScatteringPoint> Superquadric::generateScatteringPoints(
    double rhoSP) {
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
        distance +=
            std::pow(std::abs(sp.cm[ax] / semi_axes[ax]), exponents[ax]);

      if (distance <= 1) break;
    };
  }

  return sPoints;
}