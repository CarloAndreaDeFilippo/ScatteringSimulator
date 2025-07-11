#pragma once

#include <string>
#include <vector>

#include "ScatteringPoint.hpp"

namespace Cogli1 {

// Definition of simulation box Cogli1 file
void box(const std::array<double, 3>& L, std::string filename);

}  // namespace Cogli1