#include "layout/scroll_view.hpp"

#include <fmt/base.h>

#include <algorithm>
#include <limits>

#include "basic/ui_component.hpp"
#include "foundation/foundation.hpp"

const std::vector<std::shared_ptr<UIComponent>>& ScrollView::children() const {
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

void ScrollView::layout(UIConstraints constraints) {
  if (!params_.child) {
    setSize(0, 0);
    return;
  }

  constexpr float LARGE_SIZE = std::numeric_limits<float>::infinity();  // 1e7f;
  if (params_.direction == ScrollDirection::Vertical) {
    params_.child->layout(UIConstraints::maxSize(constraints.maxWidth, LARGE_SIZE));
  } else {
    params_.child->layout(UIConstraints::maxSize(LARGE_SIZE, constraints.maxHeight));
  }

  const auto& childSize = params_.child->getSize();
  switch (params_.direction) {
    case ScrollDirection::Vertical:
      setSize(std::min(childSize.width, constraints.maxWidth), constraints.maxHeight);
      break;
    case ScrollDirection::Horizontal:
      setSize(constraints.maxWidth, std::min(childSize.height, constraints.maxHeight));
      break;
  }

  params_.child->setPosition(bounds_.x, bounds_.y);
  const auto& childBounds = params_.child->getBounds();

  float maxScrollX = std::max(0.0f, childBounds.width - bounds_.width);
  float maxScrollY = std::max(0.0f, childBounds.height - bounds_.height);

  scrollOffset_.x = std::clamp(scrollOffset_.x, 0.0f, maxScrollX);
  scrollOffset_.y = std::clamp(scrollOffset_.y, 0.0f, maxScrollY);
  targetScrollOffset_.x = std::clamp(targetScrollOffset_.x, 0.0f, maxScrollX);
  targetScrollOffset_.y = std::clamp(targetScrollOffset_.y, 0.0f, maxScrollY);

  update();
}

void ScrollView::draw(SkCanvas* canvas) {
  if (!params_.child) return;

  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);
  canvas->clipRect(SkRect::MakeWH(bounds_.width, bounds_.height));

  canvas->translate(-scrollOffset_.x, -scrollOffset_.y);
  params_.child->draw(canvas);

  canvas->restore();
  UIComponent::draw(canvas);
}
