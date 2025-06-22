#pragma once

struct UIEdgeInsets {
  float top = 0.0f;
  float left = 0.0f;
  float bottom = 0.0f;
  float right = 0.0f;

  // TODO: add setters
  float horizonal() { return left + right; }

  /**
 * @brief Calculates the total vertical inset.
 *
 * @return float Sum of the top and bottom insets.
 */
float vertical() { return top + bottom; }
};

enum class MainAxisAlignment { START };

enum class CrossAxisAlignment { START, CENTER, END };
