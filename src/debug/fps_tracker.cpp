#include "debug/fps_tracker.hpp"

FpsTracker::FpsTracker()
    : lastFrameTime(std::chrono::high_resolution_clock::now()),
      frameCount(0),
      fps(0.0),
      lastFpsUpdateTime(std::chrono::high_resolution_clock::now()) {}

void FpsTracker::updateAndLogFps() {
  auto currentTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> deltaTime = currentTime - lastFrameTime;
  lastFrameTime = currentTime;
  frameCount++;

  std::chrono::duration<double> timeSinceLastFpsUpdate = currentTime - lastFpsUpdateTime;
  if (timeSinceLastFpsUpdate.count() >= 1.0) {  // Update FPS every second
    fps = static_cast<double>(frameCount) / timeSinceLastFpsUpdate.count();
    fmt::println("FPS: {:.2f}", fps);
    frameCount = 0;
    lastFpsUpdateTime = currentTime;
  }
}
