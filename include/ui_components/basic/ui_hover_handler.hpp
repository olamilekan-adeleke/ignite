#pragma once

#include <functional>

#include "offset.hpp"

using CursorPosCallback = std::function<void(double, double)>;

class UIHoverHandler {
 public:
  virtual void onHoverEnter() noexcept { isHovered_ = true; }
  virtual void onHoverExit() noexcept { isHovered_ = false; }
  virtual void onHoverMove(Offset& offset) noexcept {}

  virtual bool shouldHandleHover() { return shouldHandleHover_; }
  virtual void setShouldHandleHover(bool shouldHandleHover) { shouldHandleHover_ = shouldHandleHover; }

  virtual void setCursorPosCallback(Offset offset) noexcept {}

 private:
  bool isHovered_ = false;
  bool shouldHandleHover_ = false;
};
