#pragma once

#include "ScatteringVector.hpp"

class ScatteringPlane {
 public:
  ScatteringVector q1Vector;
  ScatteringVector q2Vector;

  ScatteringPlane() = default;

  ScatteringPlane(const ScatteringVector& scattVec1, const ScatteringVector& scattVec2) {
    q1Vector = scattVec1;
    q2Vector = scattVec2;
  }
};