#include "ParticleSystem.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#include "ParticleShapeFactory.hpp"

ParticleSystem::ParticleSystem(const std::string& infile) {
  loadSystem(infile);
}

void ParticleSystem::addParticle(Particle&& part) {
  particles.push_back(std::move(part));
  N++;
}

void ParticleSystem::loadSystem(const std::string& infile) {
  std::ifstream file_in(infile);

  if (file_in.is_open() == false) {
    std::cout << "Error opening " << infile << "\n";
    std::exit(-1);
  }

  // First line for the box length
  file_in >> Lbox[0] >> Lbox[1] >> Lbox[2];

  std::string typeStr;

  // Other lines for the particles
  while (file_in >> typeStr) {
    Particle part;

    try {
      part.particleShape = createShape(typeStr, file_in);
    } catch (const std::exception& e) {
      std::cerr << "Error creating shape: " << e.what() << "\n";
      continue;
    }

    file_in >> part.tf.cm[0] >> part.tf.cm[1] >> part.tf.cm[2];

    file_in >> part.tf.R[0][0] >> part.tf.R[0][1] >> part.tf.R[0][2];
    file_in >> part.tf.R[1][0] >> part.tf.R[1][1] >> part.tf.R[1][2];
    file_in >> part.tf.R[2][0] >> part.tf.R[2][1] >> part.tf.R[2][2];

    addParticle(std::move(part));
  }
}