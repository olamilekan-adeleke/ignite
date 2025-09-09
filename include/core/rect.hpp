#pragma once

#include <include/core/SkRect.h>

struct Rect {
  float x, y, width, height;

  SkRect toSkRect() { return SkRect::MakeXYWH(x, y, width, height); }

  bool contains(float x, float y) const {
    return x >= this->x && y >= this->y && x <= this->x + this->width && y <= this->y + this->height;
  }
};
