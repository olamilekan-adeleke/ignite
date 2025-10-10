#include "elements/fixed_box.hpp"

#include <fmt/base.h>

#include <algorithm>

#include "color.hpp"
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

void FixedBox::layout(UIConstraints size) {
  float width = std::clamp(params_.size.width, size.minWidth, size.maxWidth);
  float height = std::clamp(params_.size.height, size.minHeight, size.maxHeight);

  const auto& child = params_.child;
  if (child) {
    child->layout(UIConstraints::fitted(width, height));

    const auto& childBounds = child->getBounds();
    auto [x, y] = computeAlignedPosition(params_.alignment, width, height, childBounds.width, childBounds.height);
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
