#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ScatteringPoint.hpp"

class ParticleShape {
 public:
  virtual ~ParticleShape() = default;

  virtual void readFromInput(std::istream& input) = 0;
  virtual void printShape() { std::cout << "Base class of ParticleShape\n"; }

  virtual double computeVolume() { return 0.; };

  virtual std::vector<ScatteringPoint> generateScatteringPoints(
      double /* rhoSP */) {
    return {};
  };
};