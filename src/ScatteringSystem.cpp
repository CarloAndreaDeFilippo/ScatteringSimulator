

#include "ScatteringSystem.hpp"

#include <omp.h>

#include "ProgressBar.hpp"

void ScatteringSystem::generateScatteringPoints(const std::vector<Particle>& particles) {
  for (auto& part : particles) {
    if (scattType == ScattType::Sq) {  // Calculate S(q)
      ScatteringPoint sp;
      sp.cm = part.tf.cm;
      scatteringPoints.push_back(sp);
      NSP++;
    } else {  // Calculate full I(q)
      std::vector<ScatteringPoint> particleScatteringPoints = part.generateScatteringPoints(rhoSP);
      scatteringPoints.insert(scatteringPoints.end(), particleScatteringPoints.begin(), particleScatteringPoints.end());
      NSP += particleScatteringPoints.size();
    }
  }
}

void ScatteringSystem::cogli2(const std::array<double, 3>& Lbox, const std::string filename, const bool append) {
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

  for (size_t sp = 0; sp < cogli2MaxSpheres; sp++)
    file_out << scatteringPoints[sp].cogli2(Lbox);

  file_out.close();
}