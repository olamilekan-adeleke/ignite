
#include <fmt/base.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "logger.hpp"
#include "skia/SkiaRenderer.hpp"
#include "ui_components/text_component.hpp"
#include "window/GLFWWindowManager.hpp"

int main() {
  GLFWWindowManager windowManager;
  SkiaRenderer skiaRenderer;

  // Initialize window
  if (!windowManager.initialize(800, 600, "Skia Playground")) {
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

  Text text = Text("Hello There", Color::Red(), 40.0f, FontWeight::Bold);

  bool needsResize = false;
  bool needsLayout = true;

  // Set resize callback - handle resize logic separately from rendering
  windowManager.setResizeCallback([&](int newWidth, int newHeight) {
    fmt::println("Resize callback: {}x{}", newWidth, newHeight);
    width = newWidth;
    height = newHeight;
    needsResize = true;
    needsLayout = true;
  });

  windowManager.setRenderCallback([&]() {
    if (needsResize) {
      skiaRenderer.resize(width, height);
      needsResize = false;
      fmt::println("Skia resized to: {}x{}", width, height);
    }

    if (needsLayout) {
      text.layout(width, height);
      needsLayout = false;
    }

    // Render frame
    skiaRenderer.beginFrame();
    auto canvas = skiaRenderer.getCanvas();

    // Just draw - no layout calculations here
    text.draw(canvas);

    skiaRenderer.endFrame();

    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  });

  // Run main loop
  windowManager.run();

  return 0;
}
