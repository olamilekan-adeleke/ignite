#include "elements/fixed_box.hpp"

#include <fmt/base.h>

#include <algorithm>
#include <cmath>

#include "rect.hpp"
#include "ui_alignment.hpp"

const std::vector<std::shared_ptr<UIComponent>>& FixedBox::children() const {
  if (params_.child) {
    if (cached_children_.empty() || cached_children_[0] != params_.child) {
      cached_children_.clear();
      cached_children_.push_back(params_.child);
    }
    return cached_children_;
  }

  if (!cached_children_.empty()) cached_children_.clear();
  return UIComponent::children();
}

float FixedBox::computeWidth() const noexcept {
  if (params_.size.isGrowWidth() && params_.size.width <= 0) return INFINITY;
  return params_.size.width;
}

float FixedBox::computeHeight() const noexcept {
  if (params_.size.isGrowHeight() && params_.size.height <= 0) return INFINITY;
  return params_.size.height;
}

void FixedBox::layout(UIConstraints size) {
  float width = std::clamp(computeWidth(), size.minWidth, size.maxWidth);
  float height = std::clamp(computeHeight(), size.minHeight, size.maxHeight);

  const auto& child = params_.child;
  if (child) {
    child->layout(UIConstraints::maxSize(width, height));
    const auto& childSize = child->getSize();

    auto [x, y] = computeAlignedPosition(params_.alignment, width, height, childSize.width, childSize.height);
    child->setPosition(x, y);

    child->updateGlobalOffset({
        getGlobalOffset().x + x,
        getGlobalOffset().y + y,
    });
  }
  setSize(width, height);
}

void FixedBox::draw(SkCanvas* canvas) {
  if (params_.child) {
    canvas->save();
    canvas->translate(bounds_.x, bounds_.y);
    params_.child->draw(canvas);
    canvas->restore();
  }

  UIComponent::draw(canvas);
}
