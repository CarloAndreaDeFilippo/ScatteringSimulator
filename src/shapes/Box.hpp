#pragma once

#include <vector>

#include "../ParticleShape.hpp"
#include "../rng.hpp"

class ScatteringPoint;

class Box : public ParticleShape {
 public:
  std::array<double, 3> axes = {{1., 1., 1.}};
  std::array<double, 3> semi_axes = {{0.5, 0.5, 0.5}};

  Box() = default;

  Box(double a, double b, double c)
      : axes{{a, b, c}}, semi_axes{{0.5 * a, 0.5 * b, 0.5 * c}} {}

  void readFromInput(std::istream& input) override {
    input >> axes[0] >> axes[1] >> axes[2];
    semi_axes[0] = 0.5 * axes[0];
    semi_axes[1] = 0.5 * axes[1];
    semi_axes[2] = 0.5 * axes[2];
  }

  void printShape() override {
    std::cout << "Box: a = " << axes[0] << ", b = " << axes[1]
              << ", c = " << axes[2] << "\n";
  }

  double computeVolume() override { return axes[0] * axes[1] * axes[2]; }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

std::vector<ScatteringPoint> Box::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  auto Npoints = static_cast<size_t>(rhoSP * volume);

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    sp.cm = {rng().randomDouble(-semi_axes[0], semi_axes[0]),
             rng().randomDouble(-semi_axes[1], semi_axes[1]),
             rng().randomDouble(-semi_axes[2], semi_axes[2])};
  }

  return sPoints;
}