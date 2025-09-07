#pragma once

#include "offset.hpp"
#include "ui_alignment.hpp"

/// LayoutConstraints
struct LayoutConstraints {
  Offset offset = Offset{0.0f, 0.0f};
  UIAlignment alignment = UIAlignment::Center;
};

struct UISize {
  float width = 0.0f;
  float height = 0.0f;
};

inline UISize operator+(const UISize &lhs, const UISize &rhs) {
  return UISize{lhs.width + rhs.width, lhs.height + rhs.height};
}

inline UISize operator-(const UISize &lhs, const UISize &rhs) {
  return UISize{lhs.width - rhs.width, lhs.height - rhs.height};
}

inline UISize operator*(const UISize &lhs, float rhs) { return UISize{lhs.width * rhs, lhs.height * rhs}; }

inline UISize operator/(const UISize &lhs, float rhs) { return UISize{lhs.width / rhs, lhs.height / rhs}; }

inline bool operator==(const UISize &lhs, const UISize &rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}
