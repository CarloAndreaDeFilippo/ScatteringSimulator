#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#include "ScatteringPoint.hpp"
#include "Transform.hpp"

class ParticleShape {
 public:
  virtual ~ParticleShape() {}
  virtual void printShape() {
    std::cout << "Base class of ParticleShape\n";
  }

  virtual double computeVolume() { return 0.; };

  virtual std::vector<ScatteringPoint> generateScatteringPoints(double /* rhoSP */) { return std::vector<ScatteringPoint>(); };

  virtual std::string molgl(const Transform& /*tf*/) { return ""; };
};

class Sphere : public ParticleShape {
 public:
  double D = 1.;
  double R = 0.5;

  Sphere() = default;

  Sphere(double diameter) {
    D = diameter;
    R = 0.5 * D;
  }

  void printShape() override {
    std::cout << "Sphere: D = " << D << ", R = " << R << "\n";
  }

  double computeVolume() override {
    return (4. * M_PI / 3.) * R * R * R;
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

class Cylinder : public ParticleShape {
 public:
  double D = 1.;
  double L = 1.;
  double R = 0.5;
  double L2 = 0.5;

  Cylinder() = default;

  Cylinder(double diameter, double length) {
    D = diameter;
    L = length;

    R = 0.5 * D;
    L2 = 0.5 * L;
  }

  void printShape() override {
    std::cout << "Cylinder: D = " << D << ", R = " << R << ", L = " << L << ", L2 = " << L2 << "\n";
  }

  double computeVolume() override {
    return M_PI * R * R * L;
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

class Spherocylinder : public ParticleShape {
 public:
  double D = 1.;
  double L = 1.;
  double R = 0.5;
  double L2 = 0.5;

  Spherocylinder() = default;

  Spherocylinder(double diameter, double length) {
    D = diameter;
    L = length;

    R = 0.5 * D;
    L2 = 0.5 * L;
  }

  void printShape() override {
    std::cout << "Spherocylinder: D = " << D << ", R = " << R << ", L = " << L << ", L2 = " << L2 << "\n";
  }

  double computeVolume() override {
    return (4. * M_PI / 3.) * R * R * R + M_PI * R * R * L;
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;

  std::string molgl(const Transform& tf) override {
    std::string output = "";

    // Disk with length L

    for (int ax = 0; ax < 3; ax++) {
      output += std::to_string(tf.cm[ax]) + " ";
    }

    for (int ax = 0; ax < 3; ax++) {
      output += std::to_string(tf.R[2][ax]) + " ";
    }

    output += "@ " + std::to_string(R) + " " + std::to_string(L) + " C[purple]\n";

    // Sphere 1 at the base

    for (int ax = 0; ax < 3; ax++) {
      output += std::to_string(tf.cm[ax] - L2 * tf.R[2][ax]) + " ";
    }

    output += "@ " + std::to_string(R) + " C[purple]\n";

    // Sphere 2 at the top

    for (int ax = 0; ax < 3; ax++) {
      output += std::to_string(tf.cm[ax] + L2 * tf.R[2][ax]) + " ";
    }

    output += "@ " + std::to_string(R) + " C[purple]";

    return output;
  }
};

class Box : public ParticleShape {
 public:
  std::array<double, 3> axes = {{1., 1., 1.}};
  std::array<double, 3> semi_axes = {{0.5, 0.5, 0.5}};

  Box() = default;

  Box(double a, double b, double c) {
    axes[0] = a;
    axes[1] = b;
    axes[2] = c;

    semi_axes[0] = 0.5 * a;
    semi_axes[1] = 0.5 * b;
    semi_axes[2] = 0.5 * c;
  }

  void printShape() override {
    std::cout << "Box: a = " << axes[0] << ", b = " << axes[1] << ", c = " << axes[2] << "\n";
  }

  double computeVolume() override {
    return axes[0] * axes[1] * axes[2];
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;

  std::string molgl(const Transform& tf) override {
    std::string output = "";

    for (int ax = 0; ax < 3; ax++) {
      output += std::to_string(tf.cm[ax]) + " ";
    }

    for (int axis1 = 0; axis1 < 3; axis1++) {
      for (int axis2 = 0; axis2 < 3; axis2++) {
        output += std::to_string(tf.R[axis1][axis2]) + " ";
      }
    }

    output += "B ";
    for (int axis = 0; axis < 3; axis++)
      output += std::to_string(axes[axis]) + " ";

    output += "C[purple]";
    return output;
  }
};

class Ellipsoid : public ParticleShape {
 public:
  std::array<double, 3> semi_axes = {{0.5, 0.5, 0.5}};

  Ellipsoid() = default;

  Ellipsoid(double a, double b, double c) {
    semi_axes[0] = a;
    semi_axes[1] = b;
    semi_axes[2] = c;
  }

  void printShape() override {
    std::cout << "Ellipsoid: a = " << semi_axes[0] << ", b = " << semi_axes[1] << ", c = " << semi_axes[2] << "\n";
  }

  double computeVolume() override {
    return (4. * M_PI / 3.) * semi_axes[0] * semi_axes[1] * semi_axes[2];
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};

class Superquadric : public ParticleShape {
 public:
  std::array<double, 3> semi_axes = {{0.5, 0.5, 0.5}};
  std::array<double, 3> exponents = {{2, 2, 2}};

  Superquadric() = default;

  Superquadric(double a, double b, double c, double r, double s, double t) {
    semi_axes[0] = a;
    semi_axes[1] = b;
    semi_axes[2] = c;

    exponents[0] = r;
    exponents[1] = s;
    exponents[2] = t;
  }

  void printShape() override {
    std::cout << "Superquadric:\n";
    std::cout << "Semiaxes: A = " << semi_axes[0] << ", B = " << semi_axes[1] << ", C = " << semi_axes[2] << "\n";
    std::cout << "Exponents: r = " << exponents[0] << ", s = " << exponents[1] << ", t = " << exponents[2] << "\n";
  }

  //! Volume of the box containing the particle
  double computeVolume() override {
    return 8. * semi_axes[0] * semi_axes[1] * semi_axes[2];
  }

  std::vector<ScatteringPoint> generateScatteringPoints(double rhoSP) override;
};