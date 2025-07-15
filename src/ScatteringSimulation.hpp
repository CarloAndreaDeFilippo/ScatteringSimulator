#pragma once

#include <string>

#include "ParticleSystem.hpp"
#include "ScatteringVector.hpp"
#include "SimulationSettings.hpp"
#include "Utilities.hpp"
#include "nlohmann/json.hpp"

class ScatteringSimulation {
 public:
  SimulationSettings simSettings;

  ScatteringSimulation() = default;

  ScatteringSimulation(const std::string& scattFile) : simSettings(scattFile) {};

  void startSimulation();
};