#pragma once

#include <fmt/base.h>

#include <functional>

#include "foundation/geometry/offset.hpp"

using CursorPosCallback = std::function<void(double, double)>;

class UIHoverHandler {
 public:
  virtual void onHoverEnter() noexcept { isHovered_ = true; }
  virtual void onHoverExit() noexcept {
    fmt::println("onHoverExit");
    isHovered_ = false;
  }
  virtual void onHoverMove(Offset& offset) noexcept {}

  virtual bool isHovered() const { return isHovered_; }
  virtual void setIsHovered(bool isHovered) { isHovered_ = isHovered; }

  virtual bool shouldHandleHover() { return shouldHandleHover_; }
  virtual void setShouldHandleHover(bool shouldHandleHover) { shouldHandleHover_ = shouldHandleHover; }

  virtual void setCursorPosCallback(Offset offset) noexcept {}

 private:
  bool isHovered_ = false;
  bool shouldHandleHover_ = false;
};
