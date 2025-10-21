#pragma once

#include <sstream>

#include "offset.hpp"
#include "rect.hpp"
#include "ui_alignment.hpp"

struct Vec2 {
  float x;
  float y;
};

/// LayoutConstraints
struct LayoutConstraints {
  Offset offset{};
  UIAlignment alignment = UIAlignment::Center;
};

// struct [[deprecated("Use UISizing instead of UISize")]] UISize {
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

  UISize combine(float width, float height) const noexcept {
    return {
        this->width + width,
        this->height + height,
    };
  }

  std::string toString() const noexcept {
    std::ostringstream os;
    os << "UISize{ w: " << width << ", h: " << height << " }";
    return os.str();
  }
};

inline std::ostream &operator<<(std::ostream &os, const UISize &size) noexcept {
  return os << "UISize{ w: " << size.width << ", h: " << size.height << " }";
}

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

struct UISizing {
  enum class Type { Fixed, Grow, Fit };

  float width = 0.0f;
  float height = 0.0f;
  Type typeW = Type::Fixed;
  Type typeH = Type::Fixed;

  operator UISize() const { return UISize{.width = width, .height = height}; }

  // ────── Constructors / Factories ──────
  constexpr UISizing() = default;

  constexpr UISizing(float w, float h, Type wType = Type::Fixed, Type hType = Type::Fixed) noexcept
      : width(w), height(h), typeW(wType), typeH(hType) {}

  static constexpr UISizing Fixed(float w, float h) noexcept { return UISizing{w, h, Type::Fixed, Type::Fixed}; }

  static constexpr UISizing Grow() noexcept { return UISizing{0, 0, Type::Grow, Type::Grow}; }

  static constexpr UISizing Fit() noexcept { return UISizing{0, 0, Type::Fit, Type::Fit}; }

  static constexpr UISizing GrowWidth(float h = 0.0f) noexcept { return UISizing{0, h, Type::Grow, Type::Fixed}; }

  static constexpr UISizing GrowHeight(float w = 0.0f) noexcept { return UISizing{w, 0, Type::Fixed, Type::Grow}; }

  static constexpr UISizing Square(float size) noexcept { return UISizing{size, size, Type::Fixed, Type::Fixed}; }

  static constexpr UISizing Zero() noexcept { return UISizing{0, 0, Type::Fixed, Type::Fixed}; }

  // ────── Query Helpers ──────
  constexpr bool isGrowWidth() const noexcept { return typeW == Type::Grow; }
  constexpr bool isGrowHeight() const noexcept { return typeH == Type::Grow; }
  constexpr bool isFitWidth() const noexcept { return typeW == Type::Fit; }
  constexpr bool isFitHeight() const noexcept { return typeH == Type::Fit; }
  constexpr bool isFixed() const noexcept { return typeW == Type::Fixed && typeH == Type::Fixed; }

  // ────── Utility ──────
  constexpr UISizing scale(float factor) const noexcept {
    return UISizing{width * factor, height * factor, typeW, typeH};
  }

  constexpr UISizing clamp(float minW, float minH, float maxW, float maxH) const noexcept {
    return UISizing{std::clamp(width, minW, maxW), std::clamp(height, minH, maxH), typeW, typeH};
  }

  std::string toString() const noexcept {
    std::ostringstream os;
    os << "UISizing{ w: " << width << ", h: " << height << ", typeW: " << static_cast<int>(typeW)
       << ", typeH: " << static_cast<int>(typeH) << " }";
    return os.str();
  }
};

// ───────────────────────────── Operators ─────────────────────────────
inline std::ostream &operator<<(std::ostream &os, const UISizing &s) noexcept { return os << s.toString(); }

constexpr bool operator==(const UISizing &lhs, const UISizing &rhs) noexcept {
  return lhs.width == rhs.width && lhs.height == rhs.height && lhs.typeW == rhs.typeW && lhs.typeH == rhs.typeH;
}

constexpr bool operator!=(const UISizing &lhs, const UISizing &rhs) noexcept { return !(lhs == rhs); }

constexpr UISizing operator+(const UISizing &lhs, const UISizing &rhs) noexcept {
  return UISizing{lhs.width + rhs.width, lhs.height + rhs.height, lhs.typeW, lhs.typeH};
}

constexpr UISizing operator-(const UISizing &lhs, const UISizing &rhs) noexcept {
  return UISizing{lhs.width - rhs.width, lhs.height - rhs.height, lhs.typeW, lhs.typeH};
}

constexpr UISizing operator*(const UISizing &lhs, float rhs) noexcept {
  return UISizing{lhs.width * rhs, lhs.height * rhs, lhs.typeW, lhs.typeH};
}

constexpr UISizing operator*(float lhs, const UISizing &rhs) noexcept { return rhs * lhs; }

constexpr UISizing operator/(const UISizing &lhs, float rhs) noexcept {
  return (rhs != 0.0f) ? UISizing{lhs.width / rhs, lhs.height / rhs, lhs.typeW, lhs.typeH}
                       : UISizing{lhs.width, lhs.height, lhs.typeW, lhs.typeH};
}

inline bool almostEqual(const UISizing &a, const UISizing &b, float eps = 1e-5f) noexcept {
  return std::fabs(a.width - b.width) <= eps && std::fabs(a.height - b.height) <= eps && a.typeW == b.typeW &&
         a.typeH == b.typeH;
}
