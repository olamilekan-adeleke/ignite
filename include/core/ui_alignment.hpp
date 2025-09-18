#pragma once

#include <cassert>
#include <cstdint>
#include <utility>

enum class CrossAxisSize : std::uint8_t { FIT, FILL };

enum class MainAxisSize : std::uint8_t { FIT, FILL };

enum class MainAxisAlignment : std::uint8_t { START, CENTER, END, STRETCH };

enum class CrossAxisAlignment : std::uint8_t { START, CENTER, END };

enum class UIAlignment : std::uint8_t {
  TopLeft,
  TopCenter,
  TopRight,
  CenterLeft,
  Center,
  CenterRight,
  BottomLeft,
  BottomCenter,
  BottomRight,
};

inline std::pair<float, float> computeAlignedPosition(
    UIAlignment alignment, float width, float height, float childWidth, float childHeight) {
  float x = 0.0F;
  float y = 0.0F;
  float divisor = 2.0F;

  switch (alignment) {
    case UIAlignment::TopLeft:
      x = 0;
      y = 0;
      break;
    case UIAlignment::TopCenter:
      x = (width - childWidth) / divisor;
      y = 0;
      break;
    case UIAlignment::TopRight:
      x = width - childWidth;
      y = 0;
      break;
    case UIAlignment::CenterLeft:
      x = 0;
      y = (height - childHeight) / divisor;
      break;
    case UIAlignment::Center:
      x = (width - childWidth) / divisor;
      y = (height - childHeight) / divisor;
      break;
    case UIAlignment::CenterRight:
      x = width - childWidth;
      y = (height - childHeight) / divisor;
      break;
    case UIAlignment::BottomLeft:
      x = 0;
      y = height - childHeight;
      break;
    case UIAlignment::BottomCenter:
      x = (width - childWidth) / divisor;
      y = height - childHeight;
      break;
    case UIAlignment::BottomRight:
      x = width - childWidth;
      y = height - childHeight;
      break;
    default:
      assert(false);
  }

  return {x, y};
}
