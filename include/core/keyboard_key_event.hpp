#pragma once

#include <GLFW/glfw3.h>

enum class KeyboardKey {
  ESCAPE = GLFW_KEY_ESCAPE,
  ENTER = GLFW_KEY_ENTER,
  TAB = GLFW_KEY_TAB,
  LEFT = GLFW_KEY_LEFT,
  RIGHT = GLFW_KEY_RIGHT,
  UP = GLFW_KEY_UP,
  DOWN = GLFW_KEY_DOWN,
  BACKSPACE = GLFW_KEY_BACKSPACE,

  UNKNOWN = -1,
};

enum class KeyAction {
  RELEASE = 0,  // released
  PRESS = 1,    // pressed
  REPEAT = 2    // repeated key
};

struct KeyEvent {
  KeyboardKey key;
  KeyAction action;
  int mods;
};
