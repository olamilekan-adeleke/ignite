#pragma once

#include <include/core/SkCanvas.h>
#include "basic/diagnosable.hpp"
#include "key.hpp"
#include "rect.hpp"
#include "size.hpp"

class UIRenderObject : public Diagnosable {
 public:
  UIRenderObject() : key_(UIKey()) {}
  virtual ~UIRenderObject() = default;

  // Required overrides for concrete implementations
  virtual void layout(UISize availableSize) = 0;
  virtual void draw(SkCanvas* canvas) = 0;

  // New intrinsic size method - returns preferred size without constraints
  virtual UISize getIntrinsicSize(UIConstraints constraints) noexcept { return UISize{0.0f, 0.0f}; }

  UIRect getBounds() const { return bounds_; }
  void setPosition(float x, float y) {
    bounds_.x = x;
    bounds_.y = y;
  }
  void setSize(float w, float h) {
    bounds_.width = w;
    bounds_.height = h;
  }

  UIKey key() const { return key_; }
  void setKey(UIKey key) { key_ = std::move(key); }

  virtual bool wantsToFillMainAxis() const { return false; }
  virtual bool wantsToFillCrossAxis() const { return false; }

 private:
  void initializeDebugPaint();

 protected:
  UIRect bounds_;
  UIKey key_;

  static SkPaint debug_border_paint_;
  static bool debug_paint_initialized_;

  void debugFillProperties(std::ostringstream& os, int indent) const override;
};
