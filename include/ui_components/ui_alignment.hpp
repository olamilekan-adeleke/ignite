#pragma once

#include <cassert>
#include <utility>

enum class MainAxisAlignment { START, SPACE_BETWEEN };

enum class CrossAxisAlignment { START, CENTER, END };

enum class UIAlignment : int {
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
  float x = 0.0f;
  float y = 0.0f;

  switch (alignment) {
    case UIAlignment::TopLeft:
      x = 0;
      y = 0;
      break;
    case UIAlignment::TopCenter:
      x = (parentWidth - childWidth) / 2.0f;
      y = 0;
      break;
    case UIAlignment::TopRight:
      x = parentWidth - childWidth;
      y = 0;
      break;
    case UIAlignment::CenterLeft:
      x = 0;
      y = (parentHeight - childHeight) / 2.0f;
      break;
    case UIAlignment::Center:
      x = (parentWidth - childWidth) / 2.0f;
      y = (parentHeight - childHeight) / 2.0f;
      break;
    case UIAlignment::CenterRight:
      x = parentWidth - childWidth;
      y = (parentHeight - childHeight) / 2.0f;
      break;
    case UIAlignment::BottomLeft:
      x = 0;
      y = parentHeight - childHeight;
      break;
    case UIAlignment::BottomCenter:
      x = (parentWidth - childWidth) / 2.0f;
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
