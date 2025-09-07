#pragma once

struct Offset {
  float x = 0.0f;
  float y = 0.0f;
};

inline Offset operator+(const Offset &lhs, const Offset &rhs) { return Offset{lhs.x + rhs.x, lhs.y + rhs.y}; }

inline Offset operator-(const Offset &lhs, const Offset &rhs) { return Offset{lhs.x - rhs.x, lhs.y - rhs.y}; }

inline Offset operator*(const Offset &lhs, float rhs) { return Offset{lhs.x * rhs, lhs.y * rhs}; }

inline Offset operator/(const Offset &lhs, float rhs) { return Offset{lhs.x / rhs, lhs.y / rhs}; }
