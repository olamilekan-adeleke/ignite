#pragma once

#include <fmt/format.h>
#include <string>
#include <sstream>

struct UIEdgeInsets {
  float top = 0.0f;
  float left = 0.0f;
  float bottom = 0.0f;
  float right = 0.0f;

  static UIEdgeInsets horizonal(float horizonalSpace) {
    return UIEdgeInsets{.left = horizonalSpace, .right = horizonalSpace};
  }

  static UIEdgeInsets vertical(float verticalSpace) {
    return UIEdgeInsets{.top = verticalSpace, .bottom = verticalSpace};
  }

  float horizonal() const { return left + right; }

  float vertical() const { return top + bottom; }

  std::string toString() const noexcept {
    std::ostringstream os;
    os << "UIEdgeInsets { ";
    os << fmt::format("top: {}, left: {}, bottom: {}, right: {}", top, left, bottom, right);
    os << "}";
    return os.str();
  }
};
