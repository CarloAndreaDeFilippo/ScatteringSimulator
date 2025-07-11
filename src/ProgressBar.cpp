#include "ProgressBar.hpp"

#include <iostream>

void ProgressBar::setProgress(double prog) {
  progress = prog;
}

void ProgressBar::printBar() {
  std::cout << "[";
  for (int i = 0; i < (int)progress + 1; i++) std::cout << "|";
  for (int i = 0; i < 100 - (int)progress; i++) std::cout << " ";
  std::cout << "] " << progress << "%\r";
  std::cout.flush();
}

void ProgressBar::update() {
  if ((int)(100. * progress) % 100 == 0) printBar();
}
