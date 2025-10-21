#include <GLFW/glfw3.h>

#include "foundation/foundation.hpp"
#include "ui_manager.hpp"

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);

      // Get the window's content scale to adjust for DPI
      float xscale, yscale;
      glfwGetWindowContentScale(window, &xscale, &yscale);

      // Apply DPI scaling to coordinates
      float x = static_cast<float>(xpos) * xscale;
      float y = static_cast<float>(ypos) * yscale;

      UITapEvent event(x, y);
      UIManager::instance().sendTapEvent(event);
    }
  }
}

void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
  // Get the window's content scale to adjust for DPI
  float xscale, yscale;
  glfwGetWindowContentScale(window, &xscale, &yscale);

  // Apply DPI scaling to coordinates
  float x = static_cast<float>(xpos) * xscale;
  float y = static_cast<float>(ypos) * yscale;

  // TODO: I would probably remove later, i do not have plans to implement
}

// Optional: Scroll callback
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  // Get the window's content scale to adjust for DPI
  float xscale, yscale;
  glfwGetWindowContentScale(window, &xscale, &yscale);

  // Apply DPI scaling to coordinates
  float x = static_cast<float>(xpos) * xscale;
  float y = static_cast<float>(ypos) * yscale;

  fmt::println("Scroll at ({}, {}) - offset: ({}, {})", x, y, xoffset, yoffset);
  Offset offset{static_cast<float>(xoffset), static_cast<float>(yoffset)};
  UIManager::instance().setScrollCallback(offset);
}
