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

UISize FixedBox::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize size{};

  UIConstraints preferredConstraint = constraints;
  if (params_.size.width >= 0) {
    preferredConstraint.minWidth = preferredConstraint.minWidth = params_.size.width;
  }
  if (params_.size.height >= 0) {
    preferredConstraint.minHeight = preferredConstraint.minWidth = params_.size.height;
  }

  if (params_.child) {
    size = params_.child->getIntrinsicSize(preferredConstraint);
  } else {
    size.width = preferredConstraint.minWidth;
    size.height = preferredConstraint.minHeight;
  }

  return size;
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
    child->layout(UIConstraints::fitted(width, height));
    const auto& childSize = child->getSize();

    auto [x, y] = computeAlignedPosition(params_.alignment, width, height, childSize.width, childSize.height);
    child->setPosition(x, y);
  }
  setSize(width, height);
}

void FixedBox::draw(SkCanvas* canvas) {
  SkPaint paint;
  paint.setColor(params_.color);
  paint.setStyle(SkPaint::kFill_Style);

  SkRect bounds = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
  canvas->drawRect(bounds, paint);

  if (params_.child) {
    canvas->save();
    canvas->translate(bounds_.x, bounds_.y);
    params_.child->draw(canvas);
    canvas->restore();
  }

  UIComponent::draw(canvas);
}
