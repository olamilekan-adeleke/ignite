#include "elements/fixed_box.hpp"

#include <fmt/base.h>

#include <algorithm>
#include <cmath>

#include "foundation/foundation.hpp"
#include "render/render_object.hpp"

float FixedBoxRenderObject::computeWidth() const noexcept {
  if (params_.size.isGrowWidth() && params_.size.width <= 0) return INFINITY;
  return params_.size.width;
}

float FixedBoxRenderObject::computeHeight() const noexcept {
  if (params_.size.isGrowHeight() && params_.size.height <= 0) return INFINITY;
  return params_.size.height;
}

void FixedBoxRenderObject::performLayout(UIConstraints size) noexcept {
  float width = std::clamp(computeWidth(), size.minWidth, size.maxWidth);
  float height = std::clamp(computeHeight(), size.minHeight, size.maxHeight);

  const auto& child = children_.empty() ? nullptr : children_.front();
  if (child) {
    child->performLayout(UIConstraints::maxSize(width, height));
    const auto& childSize = child->getSize();

    auto [x, y] = computeAlignedPosition(params_.alignment, width, height, childSize.width, childSize.height);
    child->setPosition(x, y);

    // child->updateGlobalOffset({
    //     getGlobalOffset().x + x,
    //     getGlobalOffset().y + y,
    // });
  }
  setSize(width, height);
}

void FixedBoxRenderObject::paint(SkCanvas* canvas) noexcept {
  const auto& child = children_.empty() ? nullptr : children_.front();
  if (child) {
    canvas->save();
    const auto& bounds = getBounds();
    canvas->translate(bounds.x, bounds.y);
    child->paint(canvas);
    canvas->restore();
  }

  RenderObject::paint(canvas);
}
