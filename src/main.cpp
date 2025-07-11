#include <complex>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Cogli1.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"
#include "ScatteringSimulation.hpp"
#include "ScatteringSystem.hpp"
#include "rng.hpp"

int main() {
  ScatteringSimulation scattSim;

  scattSim.loadConfig("config.json");

  // Read the names of the files

  std::vector<std::string> configurations;

  std::string file_names = "file_names_scattering.txt";
  std::ifstream file_in;

  file_in.open(file_names);

  if (file_in.is_open() == false) {
    std::cout << "Error opening " << file_names << "\n";
    std::exit(-1);
  }

  std::string line;

  while (getline(file_in, line)) {
    configurations.push_back(line);
  }

  file_in.close();

  std::cout << "#Total configurations: " << configurations.size() << "\n";

  for (size_t n_conf = 0; n_conf < configurations.size(); n_conf++) {
    std::string conf_name = configurations[n_conf];

    std::cout << "Configuration " << conf_name << std::endl;

    std::string conf_string = std::to_string(n_conf);
    ParticleSystem partSys;

    partSys.loadSystem(conf_name);

    ScatteringSystem scattSys;

    // scattSys.rhoSP = 50.;

    scattSys.generateScatteringPoints(partSys.particles);

    /*
    scattSys.loadQ("scattSettings.txt");

    scattSys.dq[scattSys.axis1] = 2. * M_PI / partSys.Lbox[scattSys.axis1];
    scattSys.qqmax[scattSys.axis1] = (scattSys.qmax[scattSys.axis1] - scattSys.qmin[scattSys.axis1]) / scattSys.dq[scattSys.axis1];

    scattSys.dq[scattSys.axis2] = 2. * M_PI / partSys.Lbox[scattSys.axis2];
    scattSys.qqmax[scattSys.axis2] = (scattSys.qmax[scattSys.axis2] - scattSys.qmin[scattSys.axis2]) / scattSys.dq[scattSys.axis2];

    */

    // Cogli1 scattering points export

    // Cogli1::box(partSys.Lbox, "Cogli1/" + conf_string + ".mgl");
    // scattSys.cogli1(partSys.Lbox, "Cogli1/" + conf_string + ".mgl", true);

    // partSys.molgl("Molgl/" + conf_string + ".txt", false);

    std::array<double, 3> axis1Q = {1., 0., 0.};
    std::array<double, 3> axis2Q = {0., 0., 1.};

    double dq_min_1 = 2. * M_PI / partSys.Lbox[0];
    double dq_min_2 = 2. * M_PI / partSys.Lbox[2];
    double qmin = 0.;
    double qmax = 3.;

    size_t qVals = 1000;
    std::vector<double> valuesQ;

    /*
    bool logScale = false;

    if (logScale) {
      double log_min = std::log10(qmin);
      double log_max = std::log10(qmax);

      double log_dq = (log_max - log_min) / (qVals - 1);

      std::cout << "log_dq: " << log_dq << "\n";
      std::cout << "dq_min: " << dq_min << "\n";

      double first_dq = std::pow(10.0, log_min + log_dq) - std::pow(10.0, log_min);

      std::cout << "Prova: " << first_dq << "\n";

      if (first_dq < dq_min) {
        std::cout << "dq too little, using dq_min\n";
        log_dq = std::log10(dq_min / qmin + 1.);
        std::cout << "log_dq: " << log_dq << "\n";
      }

      // valuesQ.reserve(qVals);

      for (size_t i = 0; i < qVals; ++i) {
        double exponent = log_min + i * log_dq;
        double value = std::pow(10.0, exponent);

        if (value > qmax) break;

        valuesQ.push_back(value);
      }
    } else {
    */

    double dq = (qmax - qmin) / (qVals - 1.);

    if (dq < dq_min_1) {
      std::cout << "dq too little, using dq_min\n";
      dq = dq_min_1;
    }

    valuesQ.reserve(qVals);

    for (size_t qq = 0; qq < qVals; qq++) {
      double q = qmin + qq * dq;

      if (q > qmax) break;

      valuesQ.push_back(q);
    }

    Rho1D rho1d_1(axis1Q, valuesQ);
    rho1d_1.calculateRho(scattSys.scatteringPoints);
    rho1d_1.exportData(scattSys.NSP, "Data/rho1D_1/" + conf_string + ".txt");

    dq = (qmax - qmin) / (qVals - 1.);

    if (dq < dq_min_2) {
      std::cout << "dq too little, using dq_min\n";
      dq = dq_min_2;
    }

    valuesQ.clear();

    valuesQ.reserve(qVals);

    for (size_t qq = 0; qq < qVals; qq++) {
      double q = qmin + qq * dq;

      if (q > qmax) break;

      valuesQ.push_back(q);
    }

    Rho1D rho1d_2(axis2Q, valuesQ);
    rho1d_2.calculateRho(scattSys.scatteringPoints);
    rho1d_2.exportData(scattSys.NSP, "Data/rho1D_2/" + conf_string + ".txt");

    /*
  scattSys.initializeRho();

  scattSys.calculateRho();

  scattSys.rho2D.calculateConjugates();

  scattSys.rho2D.exportData(scattSys.NSP, "Data/rho2D/" + conf_string + ".txt");

  scattSys.calculateRho1D();

  */

    // scattSys.rho1D_1.exportData(scattSys.NSP, "Data/rho1D_1/" + conf_string + ".txt");
    // scattSys.rho1D_2.exportData(scattSys.NSP, "Data/rho1D_2/" + conf_string + ".txt");

    std::cout << std::endl;
  }

  return 0.;
}