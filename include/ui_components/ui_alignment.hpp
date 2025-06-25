#pragma once

#include <cassert>
#include <utility>

enum class MainAxisAlignment : std::uint8_t { START, SPACE_BETWEEN };

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
    UIAlignment alignment, float parentWidth, float parentHeight, float childWidth, float childHeight) {
  float x = 0.0F;
  float y = 0.0F;
  float divisor = 2.0F;

  switch (alignment) {
    case UIAlignment::TopLeft:
      x = 0;
      y = 0;
      break;
    case UIAlignment::TopCenter:
      x = (parentWidth - childWidth) / divisor;
      y = 0;
      break;
    case UIAlignment::TopRight:
      x = parentWidth - childWidth;
      y = 0;
      break;
    case UIAlignment::CenterLeft:
      x = 0;
      y = (parentHeight - childHeight) / divisor;
      break;
    case UIAlignment::Center:
      x = (parentWidth - childWidth) / divisor;
      y = (parentHeight - childHeight) / divisor;
      break;
    case UIAlignment::CenterRight:
      x = parentWidth - childWidth;
      y = (parentHeight - childHeight) / divisor;
      break;
    case UIAlignment::BottomLeft:
      x = 0;
      y = parentHeight - childHeight;
      break;
    case UIAlignment::BottomCenter:
      x = (parentWidth - childWidth) / divisor;
      y = parentHeight - childHeight;
      break;
    case UIAlignment::BottomRight:
      x = parentWidth - childWidth;
      y = parentHeight - childHeight;
      break;
    default:
      assert(false);
  }

  return {x, y};
}
