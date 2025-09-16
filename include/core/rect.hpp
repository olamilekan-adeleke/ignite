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

struct UIRect {
  float x, y, width, height;

  SkRect toSkRect() { return SkRect::MakeXYWH(x, y, width, height); }

  bool contains(float x, float y) const {
    return x >= this->x && y >= this->y && x <= this->x + this->width && y <= this->y + this->height;
  }

  UIRect copyWith(const UIRectParams &params) const {
    UIRect newRect = *this;
    if (params.x) newRect.x = *params.x;
    if (params.y) newRect.y = *params.y;
    if (params.width) newRect.width = *params.width;
    if (params.height) newRect.height = *params.height;

    return newRect;
  }

  std::string toString() const { return fmt::format("x: {}, y: {}, w: {}, h: {}", x, y, width, height); }
};
