#include "elements/separator.hpp"

#include "foundation/foundation.hpp"

void SeparatorRenderObject::performLayout(UIConstraints size) noexcept {
  if (params_.axis == Axis::VERTICAL) {
    setSize(params_.thinkness, size.maxHeight);
  } else if (params_.axis == Axis::HORIZONTAL) {
    setSize(size.maxWidth, params_.thinkness);
  }
}

void SeparatorRenderObject::paint(SkCanvas* canvas) noexcept {
  SkPaint paint;
  paint.setColor(params_.backgroundColor);
  paint.setStyle(SkPaint::kFill_Style);

  const UIRect& bounds_ = getBounds();
  canvas->drawRect(bounds_.toSkRect(), paint);
}
