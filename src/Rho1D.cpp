#include "Rho1D.hpp"

#include "ProgressBar.hpp"

void Rho1D::initialize(const double dQ, const double Qmin, const size_t QQmax) {
  dq = dQ;
  qqmax = QQmax;
  qmin = Qmin;

  rho.resize(qqmax, 0.);
}

void Rho1D::calculateRho(const std::vector<ScatteringPoint>& scatteringPoints) {
  std::complex<double> im(0.0, 1.0);  // definition of i

  ProgressBar pbar;

  int printStep = ((double)qValues.size()) / 100.;

  if (printStep < 1) printStep = 1;

#pragma omp parallel for
  for (size_t qq = 0; qq < qValues.size(); qq++) {
    std::complex<double> sum = 0.;

    for (auto& sp : scatteringPoints) {
      sum += exp(-im * dotProduct(qAxis, sp.cm) * qValues[qq]);
    }

    rho[qq] += sum;

    if (qq % printStep == 0) {
      pbar.setProgress(100. * qq / ((double)qValues.size()));

      pbar.update();
    }
  }
}

void Rho1D::exportData(const size_t NSP, const std::string filename) {
  std::ofstream file_out;
  file_out.open(filename);

  if (file_out.is_open() == false) {
    std::cout << "Error opening " + filename + "\n";
    std::exit(-1);
  }

  for (size_t qq = 0; qq < qValues.size(); qq++) {
    double q = qValues[qq];

    file_out << q << " " << std::norm(rho[qq]) / ((double)NSP) << "\n";
  }
}
