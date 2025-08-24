#pragma once

#include <string>

#include "SimulationSettings.hpp"

class ScatteringSimulation {
 public:
  SimulationSettings simSettings;

  ScatteringSimulation() = default;

  ScatteringSimulation(const std::string& scattFile) : simSettings(scattFile) {};

  void startSimulation();
};