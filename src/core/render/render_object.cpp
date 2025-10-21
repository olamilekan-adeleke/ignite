#include "core/render/render_object.hpp"

#include <include/core/SkCanvas.h>

#include "ui_manager.hpp"

SkPaint RenderObject::debug_border_paint_;
bool RenderObject::debug_paint_initialized_ = false;

void RenderObject::paint(SkCanvas* canvas) noexcept {
  if (Debug::ui_debug_mode) {
    initializeDebugPaint();

    SkRect rect = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
    canvas->drawRect(rect, debug_border_paint_);
  }
}

void RenderObject::initializeDebugPaint() {
  if (!debug_paint_initialized_) {
    debug_border_paint_.setColor(Color::Random());
    debug_border_paint_.setStyle(SkPaint::kStroke_Style);
    debug_border_paint_.setStrokeWidth(2.0f);
    debug_paint_initialized_ = true;
  } else {
    // debug_border_paint_.setColor(Color::Random());
  }
}
