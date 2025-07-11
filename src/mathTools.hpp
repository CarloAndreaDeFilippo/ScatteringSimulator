#pragma once

#include <array>
#include <cmath>

// sign function (neg -> -1, pos -> +1)
template <typename T>
inline int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

inline double vectorNorm(const std::array<double, 3>& vec) {
  return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
};

inline double dotProduct(std::array<double, 3> vec1, std::array<double, 3> vec2) {
  return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
};
