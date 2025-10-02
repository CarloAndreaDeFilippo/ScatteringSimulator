#include "Rho2D.hpp"

#include <iostream>
#include <thread>

#include "ProgressBar.hpp"
#include "mathTools.hpp"

void Rho2D::calculateRho(const std::vector<ScatteringPoint>& scatteringPoints) {
  std::complex<double> im(0.0, 1.0);  // definition of i

  auto& q1Vec = qPlane.q1Vector;
  auto& q2Vec = qPlane.q2Vector;

  //? Fix or remove the progressbar?
  ProgressBar pbar;

  int printStep = static_cast<double>(q1Vec.qqmax) / 100.;

  if (printStep < 1) printStep = 1;

#pragma omp parallel for
  for (size_t qq1 = 0; qq1 < q1Vec.qqmax; ++qq1) {
    double q1Module = q1Vec.qValues[qq1];
    for (size_t qq2 = 0; qq2 < q2Vec.qqmax; ++qq2) {
      std::complex<double> sumPosPos = 0.;
      std::complex<double> sumPosNeg = 0.;
      double q2Module = q2Vec.qValues[qq2];

      for (auto& sp : scatteringPoints) {
        double projection1 = dotProduct(q1Vec.qAxis, sp.cm) * q1Module;
        double projection2 = dotProduct(q2Vec.qAxis, sp.cm) * q2Module;
        sumPosPos += std::exp(-im * (projection1 + projection2));
        sumPosNeg += std::exp(-im * (projection1 - projection2));
      }

      pos_pos[qq1][qq2] += sumPosPos;
      pos_neg[qq1][qq2] += sumPosNeg;
    }

    if (qq1 % printStep == 0) {
      pbar.setProgress(100. * qq1 / static_cast<double>(q1Vec.qqmax));
      pbar.update();
    }
  }
}

void Rho2D::exportData(const size_t NSP, const std::string& filename) {
  std::ofstream file_out;
  file_out.open(filename);

  if (file_out.is_open() == false) {
    std::cout << "Error opening " + filename + "\n";
    std::exit(-1);
  }

  auto& q1Vec = qPlane.q1Vector;
  auto& q2Vec = qPlane.q2Vector;

  // pos_pos (+q1, +q2)
  for (size_t qq1 = 0; qq1 < q1Vec.qqmax; qq1++) {
    double q1 = q1Vec.qValues[qq1];

    for (size_t qq2 = 0; qq2 < q2Vec.qqmax; qq2++) {
      double q2 = q2Vec.qValues[qq2];

      file_out << q1 << " " << q2 << " "
               << std::norm(pos_pos[qq1][qq2]) / static_cast<double>(NSP) << "\n";
    }

    file_out << "\n";
  }

  // pos_neg (+q1, -q2)
  for (size_t qq1 = 0; qq1 < q1Vec.qqmax; qq1++) {
    double q1 = q1Vec.qValues[qq1];

    for (size_t qq2 = 0; qq2 < q2Vec.qqmax; qq2++) {
      double q2 = -q2Vec.qValues[qq2];

      file_out << q1 << " " << q2 << " "
               << std::norm(pos_neg[qq1][qq2]) /
                      static_cast<double>(NSP)
               << "\n";
    }

    file_out << "\n";
  }

  // neg_pos (-q1, +q2)
  for (size_t qq1 = 0; qq1 < q1Vec.qqmax; qq1++) {
    double q1 = -q1Vec.qValues[qq1];

    for (size_t qq2 = 0; qq2 < q2Vec.qqmax; qq2++) {
      double q2 = q2Vec.qValues[qq2];

      file_out << q1 << " " << q2 << " "
               << std::norm(std::conj(pos_neg[qq1][qq2])) /
                      static_cast<double>(NSP)
               << "\n";
    }

    file_out << "\n";
  }

  // neg_neg (-q2, -q2)
  for (size_t qq1 = 0; qq1 < q1Vec.qqmax; qq1++) {
    double q1 = -q1Vec.qValues[qq1];

    for (size_t qq2 = 0; qq2 < q2Vec.qqmax; qq2++) {
      double q2 = -q2Vec.qValues[qq2];

      file_out << q1 << " " << q2 << " "
               << std::norm(std::conj(pos_pos[qq1][qq2])) /
                      static_cast<double>(NSP)
               << "\n";
    }

    file_out << "\n";
  }

  file_out.close();
}
