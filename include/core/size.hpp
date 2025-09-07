#pragma once

#include "ui_alignment.hpp"
struct Offset;

/// Offset
struct Offset {
  float x = 0.0f;
  float y = 0.0f;
};

inline Offset operator+(const Offset &lhs, const Offset &rhs) { return Offset{lhs.x + rhs.x, lhs.y + rhs.y}; }

inline Offset operator-(const Offset &lhs, const Offset &rhs) { return Offset{lhs.x - rhs.x, lhs.y - rhs.y}; }

inline Offset operator*(const Offset &lhs, float rhs) { return Offset{lhs.x * rhs, lhs.y * rhs}; }

inline Offset operator/(const Offset &lhs, float rhs) { return Offset{lhs.x / rhs, lhs.y / rhs}; }

/// LayoutConstraints
struct LayoutConstraints {
  Offset offset = Offset{0.0f, 0.0f};
  UIAlignment alignment = UIAlignment::Center;
};

/// UISize
///

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
