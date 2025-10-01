#include "Rho2D.hpp"

#include <iostream>
#include <thread>

#include "ProgressBar.hpp"

void Rho2D::calculateConjugates() {
  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++)
    for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++) {
      neg_neg[qq1][qq2] = std::conj(pos_pos[qq1][qq2]);
      neg_pos[qq1][qq2] = std::conj(pos_neg[qq1][qq2]);
    }
}

void Rho2D::exportData(size_t NSP, std::string filename) {
  std::ofstream file_out;
  file_out.open(filename);

  if (file_out.is_open() == false) {
    std::cout << "Error opening " + filename + "\n";
    std::exit(-1);
  }

  // pos_pos
  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++) {
    double q1 = qmin[axis1] + dq[axis1] * qq1;

    for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++) {
      double q2 = qmin[axis2] + dq[axis2] * qq2;

      file_out << q1 << " " << q2 << " " << std::norm(pos_pos[qq1][qq2]) / ((double)NSP) << "\n";
    }

    file_out << "\n";
  }

  // pos_neg
  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++) {
    double q1 = dq[axis1] * qq1;

    for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++) {
      double q2 = -dq[axis2] * qq2;

      file_out << q1 << " " << q2 << " "
               << std::norm(pos_neg[qq1][qq2]) /
                      ((double)NSP)
               << "\n";
    }

    file_out << "\n";
  }

  // neg_pos
  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++) {
    double q1 = -dq[axis1] * qq1;

    for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++) {
      double q2 = dq[axis2] * qq2;

      file_out << q1 << " " << q2 << " "
               << std::norm(neg_pos[qq1][qq2]) /
                      ((double)NSP)
               << "\n";
    }

    file_out << "\n";
  }

  // neg_neg
  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++) {
    double q1 = -dq[axis1] * qq1;

    for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++) {
      double q2 = -dq[axis2] * qq2;

      file_out << q1 << " " << q2 << " "
               << std::norm(neg_neg[qq1][qq2]) /
                      ((double)NSP)
               << "\n";
    }

    file_out << "\n";
  }

  file_out.close();
}
