#pragma once

#include <dirent.h>
#include <sys/stat.h>

#include <iostream>

// C++11 function to get file names inside a folder
inline std::vector<std::string> listFilesInDir(const std::string& path) {
  std::vector<std::string> fileNames;
  DIR* dir = opendir(path.c_str());
  if (dir == nullptr) {
    std::cout << "Cannot open directory: " << path << "\n";
    exit(-1);
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;
    if (name != "." && name != "..") {
      fileNames.push_back(name);
    }
  }

  closedir(dir);
  return fileNames;
}

//! Only Linux
// C++11 function to create a directory
inline void makeDirectory(const std::string& path) {
  mkdir(path.c_str(), 0755);
}