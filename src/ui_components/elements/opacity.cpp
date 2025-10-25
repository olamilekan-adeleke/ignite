#include "elements/opacity.hpp"

#include "foundation/foundation.hpp"

void OpacityRenderObject::performLayout(UIConstraints size) noexcept {
  if (auto& child = children_.front()) {
    child->performLayout(size);
    setSize(child->getSize());

    child->setPosition(getBounds().x, getBounds().y);
  } else {
    setSize({size.width, size.height});
  }
}

void OpacityRenderObject::paint(SkCanvas* canvas) noexcept {
  float opacity = std::clamp(params_.opacity, 0.0f, 1.0f);

  if (auto& child = children_.front()) {
    U8CPU alphaValue = static_cast<U8CPU>(opacity * 255.0f);
    const auto& childBounds = child->getBounds();

    SkPaint paint;
    paint.setAlpha(alphaValue);

    canvas->saveLayer(nullptr, &paint);
    child->paint(canvas);
    canvas->restore();
  }

  RenderObject::paint(canvas);
}
