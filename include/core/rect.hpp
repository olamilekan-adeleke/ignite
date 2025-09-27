#pragma once

#include <fmt/base.h>
#include <fmt/format.h>
#include <optional>
#include <string>
#include <include/core/SkRect.h>

using optional_float = std::optional<float>;

struct UIRectParams {
  optional_float x = std::nullopt;
  optional_float y = std::nullopt;
  optional_float width = std::nullopt;
  optional_float height = std::nullopt;
};

struct UIConstraints {
  float minWidth;
  float minHeight;

  UIConstraints shrinkBy(float horizontal, float vertical) const noexcept {
    return {minWidth - horizontal, minHeight - vertical};
  }
};

struct UIRect {
  float x, y, width, height;

  SkRect toSkRect() const noexcept { return SkRect::MakeXYWH(x, y, width, height); }
  SkRect toSkRectHW() const noexcept { return SkRect::MakeWH(width, height); }

  bool contains(float x, float y) const noexcept {
    return x >= this->x && y >= this->y && x <= this->x + this->width && y <= this->y + this->height;
  }

  UIRect copyWith(const UIRectParams &params) const noexcept {
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
