#pragma once

#include <array>
#include <string>
#include <vector>

namespace cogli2 {

// Definition of simulation box cogli2 file
void box(const std::array<double, 3>& Lbox, const std::string& filename);

}  // namespace cogli2