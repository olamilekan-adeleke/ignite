#include "elements/fixed_box.hpp"
#include <fmt/base.h>

UISize FixedBox::getIntrinsicSize(UIConstraints constraints) noexcept {
  const bool w_fixed = params_.size.width >= 0.f;
  const bool h_fixed = params_.size.height >= 0.f;
  UISize child_is{};

  if (params_.child) child_is = params_.child->getIntrinsicSize(constraints);

  return {
      w_fixed ? params_.size.width : (params_.child ? child_is.width : constraints.minWidth),
      h_fixed ? params_.size.height : (params_.child ? child_is.height : constraints.minHeight),
  };
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
