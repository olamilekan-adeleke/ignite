#pragma once

#include <cassert>

struct Offset {
  float x = 0.0f;
  float y = 0.0f;
};

inline constexpr Offset operator+(const Offset &lhs, const Offset &rhs) noexcept {
  return Offset{lhs.x + rhs.x, lhs.y + rhs.y};
}
inline constexpr Offset operator-(const Offset &lhs, const Offset &rhs) noexcept {
  return Offset{lhs.x - rhs.x, lhs.y - rhs.y};
}

inline constexpr Offset operator*(const Offset &lhs, float rhs) noexcept { return Offset{lhs.x * rhs, lhs.y * rhs}; }

inline constexpr Offset operator*(float lhs, const Offset &rhs) noexcept { return rhs * lhs; }

inline constexpr Offset operator/(const Offset &lhs, float rhs) noexcept {
  assert(rhs != 0.0f && "Offset division by zero");
  return Offset{lhs.x / rhs, lhs.y / rhs};
}
