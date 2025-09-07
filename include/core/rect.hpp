#pragma once

#include <include/core/SkRect.h>

struct UIRect {
  float x, y, width, height;

  SkRect toSkRect() { return SkRect::MakeXYWH(x, y, width, height); }

  bool contains(float x, float y) const {
    return x >= this->x && y >= this->y && x <= this->x + this->width && y <= this->y + this->height;
  }
};

inline bool operator==(const UIRect &lhs, const UIRect &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height;
}

inline bool operator!=(const UIRect &lhs, const UIRect &rhs) { return !(lhs == rhs); }

inline UIRect operator+(const UIRect &lhs, const UIRect &rhs) {
  return UIRect{lhs.x + rhs.x, lhs.y + rhs.y, lhs.width + rhs.width, lhs.height + rhs.height};
}

inline UIRect operator-(const UIRect &lhs, const UIRect &rhs) {
  return UIRect{lhs.x - rhs.x, lhs.y - rhs.y, lhs.width - rhs.width, lhs.height - rhs.height};
}

inline UIRect operator*(const UIRect &lhs, float rhs) {
  return UIRect{lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs};
}

inline UIRect operator/(const UIRect &lhs, float rhs) {
  return UIRect{lhs.x / rhs, lhs.y / rhs, lhs.width / rhs, lhs.height / rhs};
}
