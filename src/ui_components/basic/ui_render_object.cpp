
#include "basic/ui_render_object.hpp"
#include "color.hpp"
#include "ui_manager.hpp"
#include <sstream>

SkPaint UIRenderObject::debug_border_paint_;
bool UIRenderObject::debug_paint_initialized_ = false;

// Default draw implementation in base class
void UIRenderObject::draw(SkCanvas* canvas) {
  if (Debug::ui_debug_mode) {
    initializeDebugPaint();

    SkRect rect = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
    canvas->drawRect(rect, debug_border_paint_);
  }
}

void UIRenderObject::initializeDebugPaint() {
  if (!debug_paint_initialized_) {
    debug_border_paint_.setColor(Color::Random());
    debug_border_paint_.setStyle(SkPaint::kStroke_Style);
    debug_border_paint_.setStrokeWidth(2.0f);
    debug_paint_initialized_ = true;
  } else {
    // debug_border_paint_.setColor(Color::Random());
  }
}

// std::string UIRenderObject::toString(int indent) const {
//   std::ostringstream os;
//   std::string pad(indent, ' ');
//
//   os << pad << typeid(*this).name() << " {\n";
//   debugFillProperties(os, indent);
//
//   auto kids = children();
//   if (!kids.empty()) {
//     os << pad << "  children: [\n";
//     for (const auto& child : kids) {
//       os << child->toString(indent + 4) << "\n";
//     }
//     os << pad << "  ]\n";
//   }
//
//   os << pad << "}";
//   return os.str();
// }

void UIRenderObject::debugFillProperties(std::ostringstream& os, int indent) const {
  std::string pad(indent, ' ');
  os << pad << "key: " << key_.toString() << "\n";
  os << pad << "offset: { x: " << bounds_.x << ", y: " << bounds_.y << " }\n";
  os << pad << "size: { w:" << bounds_.width << ", h: " << bounds_.height << " }\n";
  // os << pad << "tappable: " << (tappable_ ? "true" : "false") << "\n";
}
