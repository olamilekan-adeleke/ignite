
#include <fmt/base.h>

#include <iostream>

#include "lib/user.hpp"
#include "logger.hpp"
#include "skia/SkiaRenderer.hpp"
#include "window/GLFWWindowManager.hpp"

int main() {
  GLFWWindowManager windowManager;
  SkiaRenderer skiaRenderer;

  // Initialize window
  if (!windowManager.initialize(800, 600, "Hello World - GLFW + Skia")) {
    std::cerr << "Failed to initialize window" << std::endl;
    return -1;
  }

  // Initialize Skia renderer
  int width, height;
  windowManager.getFramebufferSize(width, height);
  if (!skiaRenderer.initialize(width, height)) {
    std::cerr << "Failed to initialize Skia renderer" << std::endl;
    return -1;
  }

  // Create user page
  UserPage userPage(&skiaRenderer);

  // Set render callback
  windowManager.setRenderCallback([&]() {
    int currentWidth, currentHeight;
    windowManager.getFramebufferSize(currentWidth, currentHeight);

    fmt::println("Window size: {}x{}", currentWidth, currentHeight);
    if (currentWidth != width || currentHeight != height) {
      width = currentWidth;
      height = currentHeight;
      skiaRenderer.resize(width, height);
    }

    // Render frame
    skiaRenderer.beginFrame();
    userPage.draw();
    skiaRenderer.endFrame();

    Logger::log("End render callback");
  });

  // Run main loop
  windowManager.run();

  return 0;
}
