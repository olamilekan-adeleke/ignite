#pragma once

#include "offset.hpp"
#include "rect.hpp"
#include "ui_alignment.hpp"

/// LayoutConstraints
struct LayoutConstraints {
  Offset offset{};
  UIAlignment alignment = UIAlignment::Center;
};

struct UISize {
  float width = 0.0f;
  float height = 0.0f;

  UIConstraints toUIConstraints() const noexcept { return {width, height}; }

  UISize combineHorizontal(const UISize &other) const noexcept {
    return {
        this->width + other.width,
        std::max(this->height, other.height),
    };
  }

  UISize combineVertical(const UISize &other) const noexcept {
    return {
        std::max(this->width, other.width),
        this->height + other.height,
    };
  }

  std::string toString() const noexcept { return fmt::format("UISize{ w: {}, h: {} }", width, height); }
};

inline constexpr bool operator==(const UISize &lhs, const UISize &rhs) noexcept {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

inline constexpr bool operator!=(const UISize &lhs, const UISize &rhs) noexcept { return !(lhs == rhs); }

inline constexpr UISize operator+(const UISize &lhs, const UISize &rhs) noexcept {
  return UISize{lhs.width + rhs.width, lhs.height + rhs.height};
}

inline constexpr UISize operator-(const UISize &lhs, const UISize &rhs) noexcept {
  return UISize{lhs.width - rhs.width, lhs.height - rhs.height};
}

inline constexpr UISize operator*(const UISize &lhs, float rhs) noexcept {
  return UISize{lhs.width * rhs, lhs.height * rhs};
}

inline constexpr UISize operator*(float lhs, const UISize &rhs) noexcept { return rhs * lhs; }

inline constexpr UISize operator/(const UISize &lhs, float rhs) noexcept {
  assert(rhs != 0.0f && "UISize division by zero");
  return UISize{lhs.width / rhs, lhs.height / rhs};
}

inline bool almostEqual(const UISize &a, const UISize &b, float eps = 1e-5f) {
  return std::fabs(a.width - b.width) <= eps && std::fabs(a.height - b.height) <= eps;
}
