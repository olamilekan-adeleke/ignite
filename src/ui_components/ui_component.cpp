
#include "ui_components/ui_component.hpp"

#include "color.hpp"
#include "ui_manager.hpp"

SkPaint UIComponent::debug_border_paint_;
bool UIComponent::debug_paint_initialized_ = false;

// Default draw implementation in base class
void UIComponent::draw(SkCanvas* canvas) {
  if (Debug::ui_debug_mode) {
    initializeDebugPaint();

    SkRect rect = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
    canvas->drawRect(rect, debug_border_paint_);
  }
}

void UIComponent::initializeDebugPaint() {
  if (!debug_paint_initialized_) {
    debug_border_paint_.setColor(Color::Random());
    debug_border_paint_.setStyle(SkPaint::kStroke_Style);
    debug_border_paint_.setStrokeWidth(2.0f);
    debug_paint_initialized_ = true;
  } else {
    debug_border_paint_.setColor(Color::Random());
  }
}
