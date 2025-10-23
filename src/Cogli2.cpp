#include "Cogli2.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

void cogli2::box(const std::array<double, 3>& Lbox, const std::string& filename) {
  std::ofstream file_out;

  file_out.open(filename);

  if (file_out.is_open() == false) {
    std::cout << "Error opening " << filename << "\n";
    std::exit(-1);
  }

  constexpr int DOUBLE_DECIMAL_DIGITS = std::numeric_limits<double>::digits10 + 1;
  file_out << std::fixed << std::setprecision(DOUBLE_DECIMAL_DIGITS);

  file_out << ".Box: " << Lbox[0] << "," << Lbox[1] << "," << Lbox[2] << "\n";

  file_out.close();
}
