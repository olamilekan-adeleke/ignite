#pragma once

#include <GLFW/glfw3.h>
#include <fmt/base.h>

struct UITapEvent {
  float x;        // Global coordinates
  float y;        // Global coordinates
  bool consumed;  // Whether event has been handled

  UITapEvent(float globalX, float globalY) : x(globalX), y(globalY), consumed(false) {}
};

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
