#pragma once

#include <include/core/SkMatrix.h>

#include <algorithm>
#include <cmath>

namespace IgniteTransform {

struct Vec2 {
  float x, y;

  Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
  Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
  Vec2 operator*(float scale) const { return {x * scale, y * scale}; }
};

struct Matrix3 {
  float m[3][3];

  static Matrix3 identity() { return {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}}; }

  static Matrix3 translation(float dx, float dy) { return {{{1, 0, dx}, {0, 1, dy}, {0, 0, 1}}}; }

  static Matrix3 scaling(float sx, float sy) { return {{{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}}}; }

  static Matrix3 rotation(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);
    return {{{c, -s, 0}, {s, c, 0}, {0, 0, 1}}};
  }

  static Matrix3 skew(float sx, float sy) { return {{{1, std::tan(sx), 0}, {std::tan(sy), 1, 0}, {0, 0, 1}}}; }

  // Apply transform around a specific origin point
  static Matrix3 aroundOrigin(const Matrix3& transform, const Vec2& origin) {
    return translation(origin.x, origin.y) * transform * translation(-origin.x, -origin.y);
  }

  Matrix3 operator*(const Matrix3& other) const {
    Matrix3 result = {};
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
        for (int k = 0; k < 3; ++k) result.m[i][j] += m[i][k] * other.m[k][j];
    return result;
  }

  Vec2 transformPoint(const Vec2& p) const {
    float x = p.x * m[0][0] + p.y * m[0][1] + m[0][2];
    float y = p.x * m[1][0] + p.y * m[1][1] + m[1][2];
    return {x, y};
  }

  // Calculate matrix determinant
  float determinant() const {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  }

  // Calculate inverse matrix
  Matrix3 inverse() const {
    float det = determinant();
    if (std::abs(det) < 1e-9f) return identity();  // Singular matrix

    float invDet = 1.0f / det;
    Matrix3 result = {};

    result.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

    result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    result.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;

    result.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    result.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
    result.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    return result;
  }

  bool operator==(const Matrix3& other) const {
    constexpr float epsilon = 1e-6f;
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
        if (std::abs(m[i][j] - other.m[i][j]) > epsilon) return false;
    return true;
  }

  bool operator!=(const Matrix3& other) const { return !(*this == other); }

  SkMatrix toSkMatrix() const {
    SkMatrix matrix;
    matrix.setAll(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], 0, 0, 1);
    return matrix;
  }
};

}  // namespace IgniteTransform
