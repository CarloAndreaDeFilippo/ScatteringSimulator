#include "Rho2D.hpp"

#include <iostream>
#include <thread>

#include "ProgressBar.hpp"
#include "mathTools.hpp"

void Rho2D::calculateRhoCPU(
    const std::vector<ScatteringPoint>& scatteringPoints) {
  std::complex<double> im(0.0, 1.0);  // definition of i

  auto& q1Vec = qPlane.q1Vector;
  auto& q2Vec = qPlane.q2Vector;

  const size_t nSP = scatteringPoints.size();

  //? Fix or remove the progressbar?
  /*ProgressBar pbar;

  int printStep = static_cast<double>(q1Vec.qqmax) / 100.;

  if (printStep < 1) printStep = 1;
  */

  std::vector<double> projBase1(nSP);
  std::vector<double> projBase2(nSP);

  for (size_t i = 0; i < nSP; ++i) {
    projBase1[i] = dotProduct(q1Vec.qAxis, scatteringPoints[i].cm);
    projBase2[i] = dotProduct(q2Vec.qAxis, scatteringPoints[i].cm);
  }

#ifdef USE_OPENMP
#pragma omp parallel for
#endif
  for (long long qq1 = 0; qq1 < q1Vec.qqmax; ++qq1) {
    double q1Module = q1Vec.qValues[qq1];
    for (size_t qq2 = 0; qq2 < q2Vec.qqmax; ++qq2) {
      double q2Module = q2Vec.qValues[qq2];

      double sumPP_re = 0.0, sumPP_im = 0.0;
      double sumPN_re = 0.0, sumPN_im = 0.0;

      for (size_t i = 0; i < projBase1.size(); ++i) {
        double proj1 = projBase1[i] * q1Module;
        double proj2 = projBase2[i] * q2Module;

        double anglePP = proj1 + proj2;
        double anglePN = proj1 - proj2;

        sumPP_re += std::cos(anglePP);
        sumPP_im -= std::sin(anglePP);
        sumPN_re += std::cos(anglePN);
        sumPN_im -= std::sin(anglePN);
      }

      pos_pos[qq1][qq2] += std::complex<double>(sumPP_re, sumPP_im);
      pos_neg[qq1][qq2] += std::complex<double>(sumPN_re, sumPN_im);
    }

    /*
    if (qq1 % printStep == 0) {
      pbar.setProgress(100. * qq1 / static_cast<double>(q1Vec.qqmax));
      pbar.update();
    }
    */
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
               << std::norm(pos_pos[qq1][qq2]) / static_cast<double>(NSP)
               << "\n";
    }

    file_out << "\n";
  }

  // pos_neg (+q1, -q2)
  for (size_t qq1 = 0; qq1 < q1Vec.qqmax; qq1++) {
    double q1 = q1Vec.qValues[qq1];

    for (size_t qq2 = 0; qq2 < q2Vec.qqmax; qq2++) {
      double q2 = -q2Vec.qValues[qq2];

      file_out << q1 << " " << q2 << " "
               << std::norm(pos_neg[qq1][qq2]) / static_cast<double>(NSP)
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
