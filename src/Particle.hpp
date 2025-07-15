#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ParticleShape.hpp"
#include "ScatteringPoint.hpp"
#include "Transform.hpp"

class Particle {
 public:
  Transform tf;  // Particle position and orientation

  std::shared_ptr<ParticleShape> particleShape;  // Geometry of the particle

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) const;

  std::string molgl();
};
