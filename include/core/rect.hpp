#pragma once

#include <fmt/base.h>
#include <fmt/format.h>
#include <include/core/SkRect.h>

#include <cmath>
#include <optional>
#include <string>
#include <tuple>

#include "axis.hpp"

using optional_float = std::optional<float>;

struct UIRectParams {
  optional_float x = std::nullopt;
  optional_float y = std::nullopt;
  optional_float width = std::nullopt;
  optional_float height = std::nullopt;
};

struct UIConstraints {
  float minWidth;
  float maxWidth;
  float minHeight;
  float maxHeight;

  float width = minWidth;
  float height = minHeight;

  static UIConstraints verticallyLose(float minWidth, float maxWidth) noexcept {
    return {minWidth, maxWidth, 0.0f, INFINITY};
  }

  static UIConstraints horizontallyLose(float minHeight, float maxHeight) noexcept {
    return {0.0f, INFINITY, minHeight, maxHeight};
  }

  static UIConstraints lose(float minWidth, float maxWidth, float minHeight, float maxHeight) noexcept {
    return {minWidth, maxWidth, minHeight, maxHeight};
  }

  static UIConstraints fitted(float width, float height) noexcept { return {width, width, height, height}; }

  UIConstraints shrinkBy(float horizontal, float vertical) const noexcept {
    return {.minWidth = minWidth - horizontal, .minHeight = minHeight - vertical};
  }

  std::tuple<float, float> mainAxisSize(const Axis& axis) const noexcept {
    switch (axis) {
      case Axis::HORIZONTAL:
        return std::make_tuple(width, height);
      case Axis::VERTICAL:
        return std::make_tuple(height, width);
    }
    return std::make_tuple(0.0f, 0.0f);
  }

  std::tuple<float, float> crossAxisSize(const Axis& axis) const noexcept {
    switch (axis) {
      case Axis::HORIZONTAL:
        return std::make_tuple(height, width);
      case Axis::VERTICAL:
        return std::make_tuple(width, height);
    }
    return std::make_tuple(0.0f, 0.0f);
  }
};

struct UIRect {
  float x, y, width, height;

  SkRect toSkRect() const noexcept { return SkRect::MakeXYWH(x, y, width, height); }
  SkRect toSkRectHW() const noexcept { return SkRect::MakeWH(width, height); }

  bool contains(float x, float y) const noexcept {
    return x >= this->x && y >= this->y && x < this->x + this->width && y < this->y + this->height;
  }

  UIRect copyWith(const UIRectParams& params) const noexcept {
    UIRect newRect = *this;
    if (params.x) newRect.x = *params.x;
    if (params.y) newRect.y = *params.y;
    if (params.width) newRect.width = *params.width;
    if (params.height) newRect.height = *params.height;

    return newRect;
  }

  std::string toString() const noexcept {
    return fmt::format("UIRect{ x: {}, y: {}, w: {}, h: {} }", x, y, width, height);
  }
};
