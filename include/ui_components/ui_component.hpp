#pragma once

#include <include/core/SkCanvas.h>

#include "rect.hpp"

class UIComponent {
 public:
  virtual ~UIComponent() = default;

  virtual void layout(float parentWidth, float parentHeight) = 0;

  // Draw method: this will draw the UI element, what ever it ends up been, to the cnvas
  virtual void draw(SkCanvas* canvas) = 0;

  UIRect getBounds() const { return bounds_; }

  void setPosistion(float x, float y) {
    bounds_.x = x;
    bounds_.y = y;
  }

  void setSize(float w, float h) {
    bounds_.width = w;
    bounds_.height = h;
  }

 protected:
  UIRect bounds_;
};
