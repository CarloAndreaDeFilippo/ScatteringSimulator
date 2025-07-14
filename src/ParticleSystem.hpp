#pragma once

#include <array>
#include <string>
#include <vector>

#include "Particle.hpp"

class ParticleSystem {
 public:
  size_t N = 0;  // Number of particles
  std::vector<Particle> particles;

  std::array<double, 3> Lbox = {{1., 1., 1.}};

  ParticleSystem() = default;
  ParticleSystem(const std::string& infile);

  void addParticle(const Particle& part);
  void loadSystem(const std::string& infile);
  void molgl(const std::string& filename, const bool append);
};