#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <limits>

// sign function (neg -> -1, pos -> +1)
template <typename T>
inline int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

template <size_t n>
inline double vectorNorm(const std::array<double, n>& vec) {
  double norm = 0.;
  for (const auto& el : vec) norm += el * el;
  return sqrt(norm);
};

template <size_t n>
inline void normalizeVector(std::array<double, n>& vec) {
  double norm = vectorNorm(vec);

  if (norm < std::numeric_limits<double>::denorm_min()) {
    std::cout << "Norm of the vector is zero, cannot normalize.\n";
    exit(-1);
  }

  for (auto& el : vec) el /= norm;
};

inline double dotProduct(std::array<double, 3> vec1, std::array<double, 3> vec2) {
  return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
};
