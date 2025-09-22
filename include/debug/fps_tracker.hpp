#pragma once

#include <chrono>
#include <fmt/base.h>

class FpsTracker {
 public:
  FpsTracker();
  void updateAndLogFps();

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;
  int frameCount;
  double fps;
  std::chrono::time_point<std::chrono::high_resolution_clock> lastFpsUpdateTime;
};
