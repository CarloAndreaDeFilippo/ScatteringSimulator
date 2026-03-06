

#include "ScatteringSystem.hpp"

#include <limits>

#include "ProgressBar.hpp"

void ScatteringSystem::generateScatteringPoints(
    const std::vector<Particle>& particles, const std::array<double, 3>& Lbox) {
  // Case S(q)
  if (scattType == ScattType::Sq) {
    NSP = particles.size();
    scatteringPoints.resize(particles.size());
    for (size_t i = 0; i < particles.size(); ++i) {
      scatteringPoints[i].cm = particles[i].tf.cm;
    }
  } else {  // Case full I(q)
    NSP = 0;

    for (const auto& part : particles) {
      std::vector<ScatteringPoint> partSPoints =
          part.generateScatteringPoints(rhoSP, Lbox);
      NSP += partSPoints.size();

      scatteringPoints.insert(scatteringPoints.end(),
                              std::make_move_iterator(partSPoints.begin()),
                              std::make_move_iterator(partSPoints.end()));
    }
  }
}

void ScatteringSystem::cogli2(const std::array<double, 3>& Lbox,
                              const std::string& filename, const bool append) {
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

  constexpr int DOUBLE_DECIMAL_DIGITS =
      std::numeric_limits<double>::digits10 + 1;
  file_out << std::fixed << std::setprecision(DOUBLE_DECIMAL_DIGITS);

  for (size_t sp = 0; sp < cogli2MaxSpheres && sp < scatteringPoints.size();
       sp++)
    file_out << scatteringPoints[sp].cogli2(Lbox);

  file_out.close();
}