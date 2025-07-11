#pragma once

class ProgressBar {
 public:
  double progress = 0.;

  void setProgress(double prog);
  void printBar();
  void update();
};