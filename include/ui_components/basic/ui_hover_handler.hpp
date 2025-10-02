#pragma once

#include <functional>

using CursorPosCallback = std::function<void(double, double)>;

class UIHoverHandler {
 public:
  virtual void onHoverEnter() {}
  virtual void onHoverExit() {}
  virtual void onHoverMove(double xpos, double ypos) {}
};
