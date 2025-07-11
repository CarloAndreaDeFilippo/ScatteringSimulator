#include "ParticleShape.hpp"

#include "mathTools.hpp"
#include "rng.hpp"

std::vector<ScatteringPoint> Sphere::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  size_t Npoints = rhoSP * volume;

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    do {
      for (int ax = 0; ax < 3; ax++)
        sp.cm[ax] = rng.drand48(-R, R);

      if (vectorNorm(sp.cm) <= R) break;

    } while (true);
  }

  return sPoints;
}

std::vector<ScatteringPoint> Cylinder::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  size_t Npoints = rhoSP * volume;

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    do {
      sp.cm[0] = rng.drand48(-R, R);
      sp.cm[1] = rng.drand48(-R, R);
      sp.cm[2] = rng.drand48(-L2, L2);

      if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1]) <= R) break;
    } while (true);
  }

  return sPoints;
}

std::vector<ScatteringPoint> Spherocylinder::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  size_t Npoints = rhoSP * volume;

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    do {
      sp.cm[0] = rng.drand48(-R, R);
      sp.cm[1] = rng.drand48(-R, R);
      sp.cm[2] = rng.drand48(-(R + L2), R + L2);

      if (std::abs(sp.cm[2]) <= L2) {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1]) <= R) break;
      } else if (sp.cm[2] > L2) {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1] + (sp.cm[2] - L2) * (sp.cm[2] - L2)) <= R) break;
      } else if (sp.cm[2] < L2) {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1] + (sp.cm[2] + L2) * (sp.cm[2] + L2)) <= R) break;
      }

      /*else {
        if (sqrt(sp.cm[0] * sp.cm[0] + sp.cm[1] * sp.cm[1] + sp.cm[2] * sp.cm[2] - sgn(sp.cm[2]) * L2 * L2) <= R) break;
      }*/

    } while (true);
  }

  return sPoints;
}

std::vector<ScatteringPoint> Box::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  size_t Npoints = rhoSP * volume;

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    for (int ax = 0; ax < 3; ax++)
      sp.cm[ax] = rng.drand48(-semi_axes[ax], semi_axes[ax]);
  }

  return sPoints;
}

std::vector<ScatteringPoint> Ellipsoid::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  size_t Npoints = rhoSP * volume;

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    do {
      for (int ax = 0; ax < 3; ax++)
        sp.cm[ax] = rng.drand48(-semi_axes[ax], semi_axes[ax]);

      double distance = 0.;

      for (int ax = 0; ax < 3; ax++)
        distance += sp.cm[ax] * sp.cm[ax] / (semi_axes[ax] * semi_axes[ax]);

      if (distance <= 1) break;

    } while (true);
  }

  return sPoints;
}

std::vector<ScatteringPoint> Superquadric::generateScatteringPoints(double rhoSP) {
  double volume = computeVolume();

  size_t Npoints = rhoSP * volume;

  std::vector<ScatteringPoint> sPoints(Npoints);

  for (auto& sp : sPoints) {
    do {
      for (int ax = 0; ax < 3; ax++)
        sp.cm[ax] = rng.drand48(-semi_axes[ax], semi_axes[ax]);

      double distance = 0.;

      for (int ax = 0; ax < 3; ax++)
        distance += std::pow(std::abs(sp.cm[ax] / semi_axes[ax]), exponents[ax]);

      if (distance <= 1) break;

    } while (true);
  }

  return sPoints;
}