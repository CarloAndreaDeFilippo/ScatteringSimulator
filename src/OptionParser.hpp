#pragma once

class OptionParser {
 public:
  std::vector<std::array<double, 3>> axes;

  enum class SimType { OneDim,
                       TwoDim };

  SimType simType;

  OptionParser(std::string& file_name);
};

OptionParser::OptionParser(std::string& file_name) {
  std::ifstream file_in(file_name);
  if (!file_in.is_open()) {
    std::cerr << "Error opening " << file_name << "\n";
    std::exit(-1);
  }

  std::cout << "Parsing simulation options...\n";

  std::string line;
  while (std::getline(file_in, line)) {
    if (line.empty() || line[0] == '#') continue;  // Skip empty lines and comments
    std::istringstream iss(line);
    std::string key;
    iss >> key;

    std::cout << key << "\n";

    if (key == "simType") {
      std::string val;
      iss >> val;

      if (val == "1d" || val == "OneDim") {
        simType = SimType::OneDim;
      }
    }

    if (key == "axis") {
      std::array<double, 3> axis;
      iss >> axis[0] >> axis[1] >> axis[2];

      std::cout << "Axis: (" << axis[0] << ", " << axis[1] << ", " << axis[2] << ")\n";

      axes.push_back(axis);
    }

    // TODO: qmin qmax rhoSP simType (1D, 2D)
  }
  file_in.close();
}