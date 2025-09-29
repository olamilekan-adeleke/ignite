#include <fmt/base.h>

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include "debug/fps_tracker.hpp"

#include "debug/debug_log_server.hpp"
#include "skia/SkiaRenderer.hpp"
#include "ui_components/ui_manager.hpp"
#include "window/GLFWWindowManager.hpp"

#include "example/example.cpp"
#include "example/counnter_app.cpp"
#include "example/todo_list.cpp"

using std::string;

using boost::asio::async_write;
using boost::asio::buffer;
using boost::asio::ip::tcp;
using boost::system::error_code;

UIManager& uiManager = UIManager::instance();

int main() {
  TCPServer server;

  try {
    GLFWWindowManager windowManager;
    SkiaRenderer skiaRenderer;

    // Initialize window
    if (!windowManager.initialize(800, 600, "Ignite Playground")) {
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
    bool needsRedraw = true;  // New flag for redraw

    auto counter_example = std::make_shared<CounterComponent>();
    auto todoList = std::make_shared<TodoListWidget>();
    // std::shared_ptr<UIComponent> rootUI = counter_example;
    std::shared_ptr<UIComponent> rootUI = todoList;
    // std::shared_ptr<UIComponent> rootUI = rootApp;

    // FPS tracking variables
    FpsTracker fpsTracker;

    // Set resize callback - handle resize logic separately from rendering
    windowManager.setResizeCallback([&](int newWidth, int newHeight) {
      fmt::println("Resize callback: {}x{}", newWidth, newHeight);
      width = newWidth;
      height = newHeight;
      needsResize = true;
      needsLayout = true;
      needsRedraw = true;  // Redraw needed after resize
    });

    static std::string lastLog;
    windowManager.setRenderCallback([&]() {
      fpsTracker.updateAndLogFps();

      if (needsResize) {
        skiaRenderer.resize(width, height);
        fmt::println("Skia resized to: {}x{}", width, height);
        needsResize = false;
      }

      // if (needsLayout) {
      uiManager.setTree(rootUI, width, height, needsResize);
      needsLayout = false;
      needsRedraw = true;  // Redraw needed after layout
                           // }

      // if (needsRedraw) {
      skiaRenderer.beginFrame();
      auto canvas = skiaRenderer.getCanvas();

      rootUI->draw(canvas);

      skiaRenderer.endFrame();
      needsRedraw = false;
      // }

      const std::string logs = rootUI->toString(0);
      if (logs != lastLog) lastLog = logs;
      // Logger::logToFile(logs);
      // std::this_thread::sleep_for(std::chrono::milliseconds(16));
    });

    windowManager.setKeyCallback([&](int key, int scancode, int action, int mods) {
      // Set the key callback
      uiManager.sendKeyEvent(key, scancode, action, mods);
    });

    windowManager.setCursorPosCallback([&](double xpos, double ypos) {
      // Set the cursor position callback
      uiManager.sendMouseEvent(xpos, ypos);
    });

    windowManager.setCharCallback([&](unsigned int codepoint) {
      // Set the character callback
      uiManager.sendCharEvent(codepoint);
    });

    // start server
    server.start();
    server.setMessageHandler([](std::shared_ptr<tcp::socket> socket, const string& msg, const string& addr) {
      string logs = lastLog + "\n";
      async_write(*socket, buffer(logs), [socket](const error_code& error, size_t bytes_transferred) {
        if (error) {
          std::cerr << "Write error in lambda: " << error.message() << std::endl;
        } else {
          fmt::println("Sent {} bytes back to client.", bytes_transferred);
        }
      });
    });

    // Run main loop
    windowManager.run();
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    server.stop();
  }
}
