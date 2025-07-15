#include "Rho1D.hpp"

#include "ProgressBar.hpp"

void Rho1D::calculateRho(const std::vector<ScatteringPoint>& scatteringPoints) {
  std::complex<double> im(0.0, 1.0);  // definition of i

  ProgressBar pbar;

  int printStep = static_cast<double>(qVector.qqmax) / 100.;

  if (printStep < 1) printStep = 1;

#pragma omp parallel for
  for (size_t qq = 0; qq < qVector.qqmax; qq++) {
    std::complex<double> sum = 0.;

    for (auto& sp : scatteringPoints) {
      sum += std::exp(-im * dotProduct(qVector.qAxis, sp.cm) * qVector.qValues[qq]);
    }

    rho[qq] += sum;

    if (qq % printStep == 0) {
      pbar.setProgress(100. * qq / static_cast<double>(qVector.qqmax));

      pbar.update();
    }
  }
}

void Rho1D::exportData(const size_t NSP, const std::string& filename) {
  std::ofstream file_out;
  file_out.open(filename);

  if (file_out.is_open() == false) {
    std::cout << "Error opening " + filename + "\n";
    std::exit(-1);
  }

  for (size_t qq = 0; qq < qVector.qqmax; qq++) {
    double q = qVector.qValues[qq];

    file_out << q << " " << std::norm(rho[qq]) / static_cast<double>(NSP) << "\n";
  }
}
