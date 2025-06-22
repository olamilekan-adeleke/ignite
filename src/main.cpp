#include <fmt/base.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "skia/SkiaRenderer.hpp"
#include "ui.hpp"
#include "ui_components/ui_manager.hpp"
#include "ui_edge_insets.hpp"
#include "window/GLFWWindowManager.hpp"

UIManager& uiManager = UIManager::instance();

/**
 * @brief Entry point for the Skia Playground graphical application.
 *
 * Initializes the window and Skia renderer, constructs the root UI layout, and enters the main event loop. Handles window resizing and rendering updates, and manages the UI tree and drawing operations for each frame.
 *
 * @return int Returns 0 on successful exit, or -1 if initialization fails.
 */
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

  const bool isLoading = false;
  ColumnParams placeholder = {
		.spacing = 10.0f,
    .crossAxisAlignment = CrossAxisAlignment::CENTER,
		.children = {
			UI::UIView({
				.insets = { .top = 20, .left = 20, .bottom = 20, .right = 20 },
				.backgroundColor = Color::Green(),
				.borderRadius = 20,
				.child = UI::UIView({
					.insets = UIEdgeInsets({ .top = 16, .left = 20, .bottom = 16, .right = 20 }),
					.margin = UIEdgeInsets({ .top = 10, .left  = 10, .bottom = 10, .right = 10 }),
					.backgroundColor = Color::Red(),
					.borderRadius = 20,
					.antiAlias = true,
					.child = UI::ColumnView({
						.spacing = 16,
						.children = {
							UI::Text("Child In UIView Child Sub One", { .fontSize = 20.0f }),
							UI::Text("View Child Child Sub Two", { .fontSize = 20.0f }),
						},
					}),
				}),
			}),

      UI::OpacityView({
        .opacity = 0.2f,
        .child = UI::UIView({
				  .insets = { .top = 20, .left = 20, .bottom = 20, .right = 20 },
				  .backgroundColor = Color::Green(),
				  .borderRadius = 20,
          .child = UI::Text("Tappable Text here", { .fontSize = 20.0f }),
        }),
      }),

			UI::UIView({
				.insets = { .top = 20, .left = 20, .bottom = 20, .right = 20 },
				.backgroundColor = Color::Green(),
				.borderRadius = 20,
        .tappable = true,
        .onTap = []( const UITapEvent& event) {
          fmt::println("Text tapped at local coordinates: ({}, {})", event.x, event.y);
        },
        .child = UI::Text("Tappable Text here", { .fontSize = 20.0f }),
			}),

      UI::UIView({
				.insets = { .top = 20, .left = 20, .bottom = 20, .right = 20 },
        // .margin = {.left = 50},
				.backgroundColor = Color::Green(),
        .child = UI::UIImageView({
          .path = "assets/trash.png",
          .width = 80,
          .height = 80,
        }),
      }),

      UI::UIView({
        // .margin = {.left = 50},
        .child = UI::UIImageView({
          .path = "/assets/trash.png",
          .height = 30,
          .opacity = 0.2f,
        }),
      }),

      UI::UIImageView({
          .path = "/assets/test_one.jpeg",
          .width = 350,
          .height = 350,
      }),

			// UI::RowView({
			// 	.spacing = 25,
			// 	.children = {
			// 		UI::Text("First Row Body", { .fontSize = 30.0f }),
			// 		UI::Text("Second Row Body", { .fontSize = 40.0f }),
			// 	},
			// }),
			//
			// UI::RowView({
			// 	.spacing = 25,
			// 	.children = {
			// 		UI::Text("First Row Body", { .fontSize = 30.0f }),
			// 		UI::Text("Second Row Body", { .fontSize = 40.0f }),
			// 	},
			// }),
   //    
   //    UI::RowView({
   //      .spacing = 20,
   //      .children = {
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::TopLeft,
   //          .child = UI::Text("Top left text", {.fontSize = 18}),
   //        }),
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::TopCenter,
   //          .child = UI::Text("Top center text", {.fontSize = 18}),
   //        }),
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::TopRight,
   //          .child = UI::Text("Top right text", {.fontSize = 18}),
   //        }),
   //      },
   //    }),
			//
   //    UI::RowView({
   //      .spacing = 20,
   //      .children = {
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::CenterLeft,
   //          .child = UI::Text("Center left text", {.fontSize = 18}),
   //        }),
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::Center,
   //          .child = UI::Text("Center text", {.fontSize = 18}),
   //        }),
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::CenterRigh,
   //          .child = UI::Text("Center right text", {.fontSize = 18}),
   //        }),
   //      },
   //    }),
			//
   //    UI::RowView({
   //      .spacing = 20,
   //      .children = {
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::BottomLeft,
   //          .child = UI::Text("Bottom left text", {.fontSize = 18}),
   //        }),
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::BottomCenter,
   //          .child = UI::Text("Bottom Center text", {.fontSize = 18}),
   //        }),
   //        UI::SizedView({
   //          .size = {200, 100},
   //          .align = UIAlignment::BottomRight,
   //          .child = UI::Text("Botton right text", {.fontSize = 18}),
   //        }),
   //      },
   //    }),
			//
			// UI::Text("Second Body Boy with Pen in the Bag", { .fontSize = 30.0f }),
			// UI::Text("Resize callback: {}x{}", { .fontSize = 68.0f }),
			//
			// UI::ColumnView({
			// 	.spacing = 20,
			// 	.children = {
			// 		UI::Text("Child Sub One", { .fontSize = 20.0f }),
			// 		UI::Text("Child Sub Two", { .fontSize = 20.0f }),
			// 	},
			// }),
	}};

  auto rootUI = UI::ColumnView(placeholder);

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

    skiaRenderer.beginFrame();
    auto canvas = skiaRenderer.getCanvas();

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
