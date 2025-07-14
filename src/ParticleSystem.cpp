#include "ParticleSystem.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

ParticleSystem::ParticleSystem(const std::string& infile) {
  loadSystem(infile);
}

void ParticleSystem::addParticle(const Particle& part) {
  particles.push_back(part);
  N++;
}

void ParticleSystem::loadSystem(const std::string& infile) {
  std::string line;

  std::string partType;

  std::ifstream file_in(infile);

  if (file_in.is_open() == false) {
    std::cout << "Error opening " << infile << "\n";
    std::exit(-1);
  }

  // First line for the box length
  if (std::getline(file_in, line)) {
    std::istringstream iss(line);

    iss >> Lbox[0] >> Lbox[1] >> Lbox[2];
  }

  // Other lines for the particles
  while (std::getline(file_in, line)) {
    std::istringstream iss(line);

    Particle part;

    iss >> partType;

    if (partType == "SPH") {
      // std::cout << "Sphere\n";

      double D = 1.;
      iss >> D;
      part.particleShape = std::make_shared<Sphere>(Sphere(D));

    } else if (partType == "CYL") {
      // std::cout << "Cylinder\n";

      double D = 1.;
      double L = 1.;

      iss >> D >> L;
      part.particleShape = std::make_shared<Cylinder>(Cylinder(D, L));

    } else if (partType == "SPHCYL") {
      // std::cout << "Spherocylinder\n";

      double D = 1.;
      double L = 1.;

      iss >> D >> L;
      part.particleShape = std::make_shared<Spherocylinder>(Spherocylinder(D, L));

    } else if (partType == "BOX") {
      // std::cout << "Box\n";

      double a = 1.;
      double b = 1.;
      double c = 1.;

      iss >> a >> b >> c;

      part.particleShape = std::make_shared<Box>(Box(a, b, c));

    } else if (partType == "ELL") {
      // std::cout << "Ellipsoid\n";

      double a = 1.;
      double b = 1.;
      double c = 1.;

      iss >> a >> b >> c;

      part.particleShape = std::make_shared<Ellipsoid>(Ellipsoid(a, b, c));

    } else if (partType == "SQUAD") {
      // std::cout << "Superquadric\n";

      double a = 1.;
      double b = 1.;
      double c = 1.;

      double r = 2.;
      double s = 2.;
      double t = 2.;

      iss >> a >> b >> c;
      iss >> r >> s >> t;

      part.particleShape = std::make_shared<Superquadric>(Superquadric(a, b, c, r, s, t));

    } else {
      std::cout << "Unknown particle type\n";
      std::exit(-1);
    }

    iss >> part.tf.cm[0] >> part.tf.cm[1] >> part.tf.cm[2];

    iss >> part.tf.R[0][0] >> part.tf.R[0][1] >> part.tf.R[0][2];
    iss >> part.tf.R[1][0] >> part.tf.R[1][1] >> part.tf.R[1][2];
    iss >> part.tf.R[2][0] >> part.tf.R[2][1] >> part.tf.R[2][2];

    addParticle(part);
  }
}

void ParticleSystem::molgl(const std::string& filename, const bool append) {
  std::ofstream file_out;

  if (append == false) {
    file_out.open(filename);
  } else {
    file_out.open(filename, std::ios::app);
  }

  if (file_out.is_open() == false) {
    std::cout << "Error opening " << filename << "\n";
    std::exit(-1);
  }

  file_out << std::fixed << std::setprecision(16);

  for (auto& part : particles) {
    file_out << part.molgl();
  }

  file_out.close();
}
