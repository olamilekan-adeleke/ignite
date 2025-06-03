
#include <fmt/base.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "skia/SkiaRenderer.hpp"
#include "ui.hpp"
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

  // auto text = TextComponent("Hello There", Color::Red(), 40.0f, FontWeight::Bold);
  auto rootUI = UI::ColumnView({
      UI::Text("Hello", 80.0f, Color::Red()),
      UI::Text("World", 68.0f, Color::Blue()),

      UI::Text("", 68.0f, Color::Blue()),
      UI::ColumnView({
          UI::Text("Second Body", 80.0f, Color::Cyan()),
          UI::Text("Textetete", 68.0f, Color::Red()),
      }),

      UI::Text("Resize callback: {}x{}", 68.0f, Color::Blue()),
  });

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
      rootUI->layout(width, height);
      needsLayout = false;
    }

    // Render frame
    skiaRenderer.beginFrame();
    auto canvas = skiaRenderer.getCanvas();

    // Just draw - no layout calculations here
    rootUI->draw(canvas);

    skiaRenderer.endFrame();

    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  });

  // Run main loop
  windowManager.run();

  return 0;
}
