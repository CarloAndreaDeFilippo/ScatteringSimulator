#include "ScatteringVector.hpp"

#include "mathTools.hpp"

ScatteringVector::ScatteringVector(const nlohmann::json& j) {
  qAxis = {j.at("direction").at(0).get<double>(),
           j.at("direction").at(1).get<double>(),
           j.at("direction").at(2).get<double>()};

  normalizeVector(qAxis);

  if (j.contains("qmin")) qmin = j.at("qmin").get<double>();
  if (j.contains("qmax")) qmax = j.at("qmax").get<double>();
  if (j.contains("dq")) dq = j.at("dq").get<double>();

  calculateQValues();
}

void ScatteringVector::calculateQValues() {
  qqmax = static_cast<size_t>((qmax - qmin) / dq);
  qValues.clear();
  qValues.reserve(qqmax);
  for (size_t qq = 0; qq < qqmax; ++qq)
    qValues.push_back(qmin + static_cast<double>(qq) * dq);
}