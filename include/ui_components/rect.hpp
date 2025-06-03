#pragma once

#include <include/core/SkRect.h>

struct UIRect {
  float x, y, width, height;

  SkRect toSkRect() { return SkRect::MakeXYWH(x, y, width, height); }
};
