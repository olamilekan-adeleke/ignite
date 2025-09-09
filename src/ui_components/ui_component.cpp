#include "ui_component.hpp"
#include "ui_manager.hpp"
#include "color.hpp"
#include <sstream>
#include "debug/diagnosable.hpp"

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
    // debug_border_paint_.setColor(Color::Random());
  }
}

std::string UIComponent::toString(int indent) const {
  std::ostringstream os;
  std::string pad(indent, ' ');

  os << pad << typeid(*this).name() << " {\n";
  debugFillProperties(os, indent);

  if (!children().empty()) {
    os << pad << "  children: [\n";
    for (auto& child : children()) {
      os << child->toString(indent + 4) << "\n";
    }
    os << pad << "  ]\n";
  }

  os << pad << "}";
  return os.str();
}

void UIComponent::debugFillProperties(std::ostringstream& os, int indent) const {
  std::string pad(indent, ' ');
  os << pad << "key: " << key_.toString().c_str() << "\n";
  os << pad << "offset: { x: " << bounds_.x << ", y: " << bounds_.y << " }\n";
  os << pad << "size: { w:" << bounds_.width << ", h: " << bounds_.height << " }\n";
  os << pad << "tappable: " << (tappable_ ? "true" : "false") << "\n";
}
