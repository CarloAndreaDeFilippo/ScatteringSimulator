#include "Rho1D.hpp"

#include "ProgressBar.hpp"

void Rho1D::calculateRho(const std::vector<ScatteringPoint>& scatteringPoints) {
  //? Fix or remove the progressbar?
  /*ProgressBar pbar;

  int printStep = static_cast<double>(qVector.qqmax) / 100.;

  if (printStep < 1) printStep = 1;
  */

  const size_t nSP = scatteringPoints.size();

  std::vector<double> projBase(nSP);
  for (size_t i = 0; i < nSP; ++i) {
    projBase[i] = dotProduct(qVector.qAxis, scatteringPoints[i].cm);
  }

#ifdef USE_CUDA
  computeRhoCUDA(projBase);
#else
  computeRhoCPU(projBase);
#endif
}

void Rho1D::computeRhoCPU(const std::vector<double>& projBase) {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
  for (size_t qq = 0; qq < qVector.qqmax; ++qq) {
    double qModule = qVector.qValues[qq];

    double sum_re = 0.0;
    double sum_im = 0.0;

    for (auto proj : projBase) {
      double angle = proj * qModule;
      sum_re += std::cos(angle);
      sum_im -= std::sin(angle);
    }

    rho[qq] += std::complex<double>(sum_re, sum_im);

    /*
    if (qq % printStep == 0) {
      pbar.setProgress(100. * qq / static_cast<double>(qVector.qqmax));
      pbar.update();
    }
    */
  }
}

void Rho1D::exportData(const size_t NSP, const std::string& filename) {
  std::ofstream file_out;
  file_out.open(filename);

  if (file_out.is_open() == false) {
    std::cout << "Error opening " + filename + "\n";
    std::exit(-1);
  }

  for (size_t qq = 0; qq < qVector.qqmax; ++qq) {
    double q = qVector.qValues[qq];

    file_out << q << " " << std::norm(rho[qq]) / static_cast<double>(NSP) << "\n";
  }
}
