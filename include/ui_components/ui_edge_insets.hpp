#pragma once

struct UIEdgeInsets {
  float top = 0.0f;
  float left = 0.0f;
  float bottom = 0.0f;
  float right = 0.0f;

  // TODO: add setters
  float horizonal() { return left + right; }

  float vertical() { return top + bottom; }
};
