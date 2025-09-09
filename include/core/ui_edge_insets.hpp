#pragma once

struct UIEdgeInsets {
  float top = 0.0f;
  float left = 0.0f;
  float bottom = 0.0f;
  float right = 0.0f;

  // TODO: add setters
  float horizonal() const { return left + right; }

  float vertical() const { return top + bottom; }
};
