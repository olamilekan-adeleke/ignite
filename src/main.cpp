
#include <fmt/base.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "skia/SkiaRenderer.hpp"
#include "ui.hpp"
#include "ui_components/ui_manager.hpp"
#include "window/GLFWWindowManager.hpp"

int main() {
  GLFWWindowManager windowManager;
  SkiaRenderer skiaRenderer;
  UIManager &uiManager = UIManager::instance();

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

  // UI::UIView({
  //     .insets =
  //         {
  //             .top = 20,
  //             .left = 20,
  //             .bottom = 20,
  //             .right = 20,
  //         },
  //     .backgroundColor = Color::Green(),
  //     .borderRadius = 20,
  //     .child =
  //         UI::UIView({
  //             .insets = UIEdgeInsets({
  //                 .top = 16,
  //                 .left = 20,
  //                 .bottom = 16,
  //                 .right = 20,
  //             }),
  //             .margin = UIEdgeInsets({
  //                 .top = 10,
  //                 .left = 10,
  //                 .bottom = 10,
  //                 .right = 10,
  //             }),
  //             .backgroundColor = Color::Red(),
  //             .borderRadius = 20,
  //             .antiAlias = true,
  //             .child = UI::ColumnView({
  //                 .spacing = 16,
  //                 .children =
  //                     {
  //                         UI::Text("Child In UIView Child Sub One", {.fontSize = 20.0f}),
  //                         UI::Text("View Child Child Sub Two", {.fontSize = 20.0f}),
  //                     },
  //             }),
  //         }),
  // }),

  // auto text = TextComponent("Hello There", Color::Red(), 40.0f,
  // FontWeight::Bold);
  const bool isLoading = false;
  auto rootUI = UI::ColumnView({
      .spacing = 10.0f,
      .children =
          {

              UI::RowView({
                  .spacing = 25,
                  .children =
                      {
                          UI::Text("First Row Body", {.fontSize = 30.0f}),
                          UI::Text("Second Row Body", {.fontSize = 40.0f}),
                      },
              }),

              UI::Text("Second Body Boy with Pen in the Bag", {.fontSize = 30.0f}),
              UI::Text("Resize callback: {}x{}", {.fontSize = 68.0f}),

                          UI::ColumnView({
                              .spacing = 20,
                              .children =
                                  {
                                      UI::Text("Child Sub One", {.fontSize = 20.0f}),
                                      UI::Text("Child Sub Two", {.fontSize = 20.0f}),
                                  },
                          }),
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
      fmt::println("Skia resized to: {}x{}", width, height);
    }

    // if (needsLayout) {
    //   rootUI->layout(width, height);
    //   needsLayout = false;
    // }
    // rootUI->layout(width, height);
    // Render frame
    skiaRenderer.beginFrame();
    auto canvas = skiaRenderer.getCanvas();

    // Just draw - no layout calculations here
    uiManager.setTree(rootUI, width, height, needsResize);
    rootUI->draw(canvas);

    if (needsResize) {
      needsResize = false;
    }

    skiaRenderer.endFrame();

    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  });

  // Run main loop
  windowManager.run();

  return 0;
}
