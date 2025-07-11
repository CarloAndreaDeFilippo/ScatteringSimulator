#pragma once

#include <chrono>
#include <random>

class Random {
 public:
  std::mt19937 generate() {
    std::random_device rd{};

    // Create seed_seq with clock and 7 random numbers from std::random_device
    std::seed_seq ss{
        static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
        rd(), rd(), rd(), rd(), rd(), rd(), rd()};

    return std::mt19937{ss};
  }

  std::mt19937 mt{generate()};

  double drand48() {
    return std::uniform_real_distribution<double>{0., 1.}(mt);
  }

  double drand48(double a, double b) {
    // return drand48() * (b - a) + a;
    return std::uniform_real_distribution<double>{a, b}(mt);
  }

}

extern rng;
