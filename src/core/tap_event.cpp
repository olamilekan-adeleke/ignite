
#include "tap_event.hpp"
#include <GLFW/glfw3.h>
#include "ui_manager.hpp"

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);

      float x = static_cast<float>(xpos);
      float y = static_cast<float>(ypos);

      UITapEvent event(x, y);
      UIManager::instance().sendTapEvent(event);
    }
  }
}

void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
  float x = static_cast<float>(xpos);
  float y = static_cast<float>(ypos);

  // TODO: I would probably remove later, i do not have plans to implement
}

// Optional: Scroll callback
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  float x = static_cast<float>(xpos);
  float y = static_cast<float>(ypos);

  fmt::println("Scroll at ({}, {}) - offset: ({}, {})", x, y, xoffset, yoffset);

  // TODO: will come back and implement for scroll able view
}
