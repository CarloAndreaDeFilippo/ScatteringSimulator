#include "Rho2D.hpp"

#include <iostream>
#include <thread>

#include "ProgressBar.hpp"

/*
void Rho2D::loadQ(const std::string infile) {
  std::ifstream file_in(infile);

  if (file_in.is_open() == false) {
    std::cout << "Error opening " << infile << "\n";
    std::exit(-1);
  }

  file_in >> qmax[0] >> qmax[1] >> qmax[2];
  file_in >> dq[0] >> dq[1] >> dq[2];
  file_in >> axis1 >> axis2;

  for (int ax = 0; ax < 3; ax++) qqmax[ax] = qmax[ax] / dq[ax];
}*/

void Rho2D::initialize(const std::array<double, 3> dQ, const std::array<double, 3> Qmin, const std::array<size_t, 3> QQmax) {
  dq = dQ;
  qmin = Qmin;
  qqmax = QQmax;

  pos_pos.resize(qqmax[axis1]);
  pos_neg.resize(qqmax[axis1]);
  neg_pos.resize(qqmax[axis1]);
  neg_neg.resize(qqmax[axis1]);

  for (auto& rho : pos_pos)
    rho.resize(qqmax[axis2], 0.);

  for (auto& rho : pos_neg)
    rho.resize(qqmax[axis2], 0.);

  for (auto& rho : neg_pos)
    rho.resize(qqmax[axis2], 0.);

  for (auto& rho : neg_neg)
    rho.resize(qqmax[axis2], 0.);
}

/*
void Rho2D::setQ(const std::array<double, 3> Qmax, const std::array<double, 3> dQ) {
  qmax = Qmax;
  dq = dQ;

  for (int ax = 0; ax < 3; ax++) qqmax[ax] = qmax[ax] / dq[ax];
}*/

/*
void Rho2D::calculateRange(const size_t sp_in, const size_t sp_fin, const std::vector<ScatteringPoint>& scatteringPoints) {
  std::complex<double> im(0.0, 1.0);  // definition of i

  ProgressBar pbar;

  int printStep = ((double)qqmax[axis1]) / 100.;

  if (printStep < 1) printStep = 1;

  for (size_t qq1 = 0; qq1 < qqmax[axis1]; qq1++) {
    double q1 = dq[axis1] * (qq1 + 0.5);

    for (size_t qq2 = 0; qq2 < qqmax[axis2]; qq2++) {
      double q2 = dq[axis2] * (qq2 + 0.5);

      for (size_t sp = sp_in; sp < sp_fin; sp++) {
        pos_pos[qq1][qq2] +=
            exp(-im * (scatteringPoints[sp].cm[axis1] * q1 + scatteringPoints[sp].cm[axis2] * q2));

        pos_neg[qq1][qq2] +=
            exp(-im * (scatteringPoints[sp].cm[axis1] * q1 - scatteringPoints[sp].cm[axis2] * q2));
      }
    }

    if (qq1 % printStep == 0) {
      pbar.setProgress(100. * qq1 / ((double)qqmax[axis1]));

      pbar.update();
    }
  }
}

void Rho2D::calculateThreads(const std::vector<ScatteringPoint>& scatteringPoints, const int Nthreads) {
  std::vector<std::thread> threads;

  for (int nt = 0; nt < Nthreads; ++nt) {
    size_t sp_in = nt * scatteringPoints.size() / ((double)Nthreads);
    size_t sp_fin = (nt + 1) * scatteringPoints.size() / ((double)Nthreads);

    if (sp_fin >= scatteringPoints.size()) sp_fin = scatteringPoints.size() - 1;

    threads.emplace_back(&Rho2D::calculateRange, this, sp_in, sp_fin, scatteringPoints);
  }

  for (auto& thr : threads)
    thr.join();
}*/

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
