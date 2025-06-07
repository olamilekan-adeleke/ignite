#pragma once

#include <include/core/SkCanvas.h>

#include <memory>
#include <vector>

#include "rect.hpp"
#include "ui_key.hpp"

class UIComponent {
 public:
  UIComponent() : key_(UIKey()) {
  }

  virtual ~UIComponent() = default;

  virtual void layout(float parentWidth, float parentHeight) = 0;

  // Draw method: this will draw the UI element, what ever it ends up been, to
  // the cnvas
  virtual void draw(SkCanvas *canvas);

  UIRect getBounds() const {
    return bounds_;
  }

  void setPosition(float x, float y) {
    bounds_.x = x;
    bounds_.y = y;
  }

  void setSize(float w, float h) {
    bounds_.width = w;
    bounds_.height = h;
  }

  void setKey(UIKey key) {
    key_ = key;
  }
  UIKey key() const {
    return key_;
  }

  virtual const std::vector<std::shared_ptr<UIComponent>> &children() {
    return {};
  }

 protected:
  UIRect bounds_;
  UIKey key_;

  static SkPaint debug_border_paint_;
  static bool debug_paint_initialized_;

  void initializeDebugPaint();
};
