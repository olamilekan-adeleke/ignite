#include <fmt/base.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "logger.hpp"
#include "skia/SkiaRenderer.hpp"
#include "ui_components/ui_manager.hpp"
#include "window/GLFWWindowManager.hpp"
#include "example/example.cpp"
#include "example/counnter_app.cpp"

UIManager& uiManager = UIManager::instance();

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

  bool needsResize = false;
  bool needsLayout = true;

  auto counter_example = std::make_shared<CounterComponent>();
  std::shared_ptr<UIComponent> rootUI = counter_example;
  // std::shared_ptr<UIComponent> rootUI = rootApp;

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
      fmt::println("Skia resized to: {}x{}", width, height);
    }

    skiaRenderer.beginFrame();
    auto canvas = skiaRenderer.getCanvas();

    uiManager.setTree(rootUI, width, height, needsResize);
    rootUI->draw(canvas);

    if (needsResize) {
      needsResize = false;
    }

    skiaRenderer.endFrame();

    Logger::logToFile(rootUI->toString(0));
    // std::this_thread::sleep_for(std::chrono::milliseconds(16));
  });

  // Run main loop
  windowManager.run();

  return 0;
}
