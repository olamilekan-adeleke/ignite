#include "ui_components/elements/alignment_box.hpp"

#include <fmt/base.h>

#include "foundation/foundation.hpp"

void AlignmentBoxRenderer::performLayout(UIConstraints constraints) noexcept {
  float width = constraints.maxWidth;
  float height = constraints.maxHeight;

  const auto& child = getChild(0);
  if (child) {
    child->performLayout(UIConstraints::maxSize(width, height));
    const auto& childSize = child->getSize();

    auto [x, y] = computeAlignedPosition(UIAlignment::Center, width, height, childSize.width, childSize.height);
    child->setPosition(x, y);

    // child->updateGlobalOffset({ getGlobalOffset().x + x, getGlobalOffset().y + y});
  }
  setSize(width, height);
}

void AlignmentBoxRenderer::paint(SkCanvas* canvas) noexcept {
  const auto& child = getChild(0);
  if (child) {
    canvas->save();
    const auto& bounds_ = getBounds();
    canvas->translate(bounds_.x, bounds_.y);
    child->paint(canvas);
    canvas->restore();
  }

  RenderObject::paint(canvas);
}
