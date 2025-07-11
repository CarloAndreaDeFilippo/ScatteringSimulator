

#include "ScatteringSystem.hpp"

#include <omp.h>

#include "ProgressBar.hpp"

void ScatteringSystem::generateScatteringPoints(std::vector<Particle> particles) {
  for (auto& part : particles) {
    if (Sq) {  // Calculate S(q)
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

void ScatteringSystem::loadQ(const std::string infile) {
  std::ifstream file_in(infile);

  if (file_in.is_open() == false) {
    std::cout << "Error opening " << infile << "\n";
    std::exit(-1);
  }

  file_in >> axis1 >> axis2;
  file_in >> qmin[axis1] >> qmin[axis2];
  file_in >> qmax[axis1] >> qmax[axis2];

  file_in >> rhoSP;
  file_in >> Sq;  // if input != 0 -> calculate only Sq
}

void ScatteringSystem::initializeRho() {
  rho1D_1.initialize(dq[axis1], qmin[axis1], qqmax[axis1]);
  rho1D_2.initialize(dq[axis2], qmin[axis2], qqmax[axis2]);

  rho2D.axis1 = axis1;
  rho2D.axis2 = axis2;

  rho2D.initialize(dq, qmin, qqmax);
}

void ScatteringSystem::calculateRho() {
  std::complex<double> im(0.0, 1.0);  // definition of i

  ProgressBar pbar;

  int printStep = ((double)qqmax[axis1]) / 100.;

  if (printStep < 1) printStep = 1;

#pragma omp parallel for
  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++) {
    for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++) {
      double q1 = qmin[axis1] + dq[axis1] * qq1;
      double q2 = qmin[axis2] + dq[axis2] * qq2;

      std::complex<double> sumPos = 0.0;
      std::complex<double> sumNeg = 0.0;

      for (size_t sp = 0; sp < scatteringPoints.size(); sp++) {
        double dot1 = scatteringPoints[sp].cm[axis1] * q1;
        double dot2 = scatteringPoints[sp].cm[axis2] * q2;

        sumPos += exp(-im * (dot1 + dot2));
        sumNeg += exp(-im * (dot1 - dot2));
      }

      rho2D.pos_pos[qq1][qq2] += sumPos;
      rho2D.pos_neg[qq1][qq2] += sumNeg;
    }

    if (qq1 % printStep == 0) {
      pbar.setProgress(100. * qq1 / ((double)qqmax[axis1]));

      pbar.update();
    }
  }
}

void ScatteringSystem::calculateRho1D() {
  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++)
    rho1D_1.rho[qq1] = rho2D.pos_pos[qq1][0];

  for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++)
    rho1D_2.rho[qq2] = rho2D.pos_pos[0][qq2];
}

void ScatteringSystem::cogli1(const std::array<double, 3>& L, const std::string filename, const bool append) {
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

  for (auto& sp : scatteringPoints) {
    file_out << sp.cogli1(L);
  }

  file_out.close();
}