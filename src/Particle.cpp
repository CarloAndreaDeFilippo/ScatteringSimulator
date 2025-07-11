#include "Particle.hpp"

#include <string>

std::vector<ScatteringPoint> Particle::generateScatteringPoints(double rhoSP) {
  std::vector<ScatteringPoint> sPoints = particleShape->generateScatteringPoints(rhoSP);

  for (auto& sp : sPoints) {
    std::array<double, 3> new_pos = {0., 0., 0.};

    for (int ax = 0; ax < 3; ax++) {
      new_pos[ax] = tf.R[0][ax] * sp.cm[0] + tf.R[1][ax] * sp.cm[1] + tf.R[2][ax] * sp.cm[2] + tf.cm[ax];
    }

    sp.cm = new_pos;
  }

  return sPoints;
}

std::string Particle::molgl() {
  std::string output = "";

  //! ONLY DONE FOR BOXES AT THE MOMENT

  output += particleShape->molgl(tf);

  output += "\n";

  return output;
}
