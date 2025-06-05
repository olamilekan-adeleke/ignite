
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
  const bool isLoading = false;
  auto rootUI = UI::ColumnView({
      .spacing = 16.0f,
      .children =
          {
              // UI::ColumnView({
              //     .spacing = 50,
              //     .children =
              //         {
              //             UI::Text("Child Sub One", {.fontSize = 20.0f}),
              //             UI::Text("Child Sub Two", {.fontSize = 20.0f}),
              //         },
              // }),

              UI::Text("Hello", {.fontSize = 80.0f, .color = Color::Red()}),
              UI::Text("Leading (fLeading) is extra spacing you’d add if you were stacking multiple lines. In "
                       "practice, if ",
                       {
                           .color = Color::Blue(),
                           .fontSize = 20.0f,
                       }),

              UI::Text("Second Body Boy with Pen in the Bag", {.fontSize = 10.0f}),

              // UI::RowView({
              //     UI::Text("Second Body", {.fontSize = 80.0f}),
              //     UI::Text("Textetete", {.fontSize = 68.0f}),
              // }),

              UI::Text("Second Body Boy with Pen in the Bag", {.fontSize = 30.0f}),
              UI::Text("Resize callback: {}x{}", {.fontSize = 68.0f}),
          },
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

    // rootUI->layout(width, height);
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
