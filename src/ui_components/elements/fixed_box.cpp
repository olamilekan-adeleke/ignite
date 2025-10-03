#include "elements/fixed_box.hpp"

#include <fmt/base.h>

UISize FixedBox::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize size{};

  if (params_.child) size = params_.child->getIntrinsicSize(constraints);

  if (params_.size.width >= 0) size.width = params_.size.width;
  if (params_.size.height >= 0) size.height = params_.size.height;
  return size;
}

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

void FixedBox::layout(UISize size) {
  float width = params_.size.width >= 0 ? params_.size.width : size.width;
  float height = params_.size.height >= 0 ? params_.size.height : size.height;

  const auto& child = params_.child;
  if (child) {
    child->layout({width, height});

    const auto& childBounds = child->getBounds();

    auto [x, y] = computeAlignedPosition(UIAlignment::Center, width, height, childBounds.width, childBounds.height);
    child->setPosition(x, y);
  }

  bounds_.width = width;
  bounds_.height = height;
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
