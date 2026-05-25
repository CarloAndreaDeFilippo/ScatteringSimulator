#include "ParticleShapeFactory.hpp"

#include <functional>
#include <unordered_map>

#include "ParticleShape.hpp"
#include "shapes/Box.hpp"
#include "shapes/Cylinder.hpp"
#include "shapes/Ellipsoid.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Spherocylinder.hpp"
#include "shapes/Superquadric.hpp"

using ShapeFactory =
    std::function<std::unique_ptr<ParticleShape>(std::istream&)>;

template <typename ShapeType>
std::unique_ptr<ParticleShape> makeShape(std::istream& input) {
  auto shape = std::make_unique<ShapeType>();
  shape->readFromInput(input);
  return shape;
}

static const std::unordered_map<std::string, ShapeFactory> factoryMap = {
    {"SPH", &makeShape<Sphere>},
    {"CYL", &makeShape<Cylinder>},
    {"SPHCYL", &makeShape<Spherocylinder>},
    {"BOX", &makeShape<Box>},
    {"ELL", &makeShape<Ellipsoid>},
    {"SQUAD", &makeShape<Superquadric>},
};

std::unique_ptr<ParticleShape> createShape(const std::string& typeStr,
                                           std::istream& input) {
  auto it = factoryMap.find(typeStr);
  if (it == factoryMap.end()) {
    throw std::invalid_argument("Unknown shape type: " + typeStr);
  }
  return it->second(input);
}