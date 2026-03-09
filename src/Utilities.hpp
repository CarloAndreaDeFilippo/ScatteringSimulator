#pragma once

#include <dirent.h>
#include <sys/stat.h>

#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>

namespace fs = boost::filesystem;

inline std::vector<std::string> listFilesInDir(const std::string& path) {
  std::vector<std::string> fileNames;

  if (!fs::is_directory(path)) {
    std::cout << path << " is not a directory.\n";
    exit(-1);
  }

  for (const auto& fName : fs::directory_iterator(path))
    fileNames.push_back(fName.path().filename().string());

  return fileNames;
}

inline bool directoryExists(const std::string& path) {
  return fs::exists(path) && fs::is_directory(path);
}

inline void makeDirectory(const std::string& path) {
  fs::create_directories(path);
}