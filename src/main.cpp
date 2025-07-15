#include <iostream>

#include "ScatteringSimulation.hpp"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Error, choose the settings file.\n";
    exit(-1);
  }

  // Initialization of the simulation
  ScatteringSimulation scattSim(argv[1]);

  // Perform simulation
  scattSim.startSimulation();

  return 0.;
}