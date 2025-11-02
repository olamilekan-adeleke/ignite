#include "layout/scroll_view.hpp"

void ScrollViewRenderObject::performLayout(UIConstraints constraints) noexcept {
  if (this->children_.empty()) return;

  const auto& child = this->children_.front();
  if (!child) {
    setSize(0, 0);
    return;
  }

  constexpr float LARGE_SIZE = std::numeric_limits<float>::infinity();  // 1e7f;
  if (params_.direction == ScrollDirection::Vertical) {
    child->performLayout(UIConstraints::maxSize(constraints.maxWidth, LARGE_SIZE));
  } else {
    child->performLayout(UIConstraints::maxSize(LARGE_SIZE, constraints.maxHeight));
  }

  const auto& childSize = child->getSize();
  switch (params_.direction) {
    case ScrollDirection::Vertical:
      setSize(std::min(childSize.width, constraints.maxWidth), constraints.maxHeight);
      break;
    case ScrollDirection::Horizontal:
      setSize(constraints.maxWidth, std::min(childSize.height, constraints.maxHeight));
      break;
  }

  const auto& bounds_ = getBounds();
  child->setPosition(bounds_.x, bounds_.y);
  const auto& childBounds = child->getBounds();

  float maxScrollX = std::max(0.0f, childBounds.width - bounds_.width);
  float maxScrollY = std::max(0.0f, childBounds.height - bounds_.height);

  scrollOffset_.x = std::clamp(scrollOffset_.x, 0.0f, maxScrollX);
  scrollOffset_.y = std::clamp(scrollOffset_.y, 0.0f, maxScrollY);
  targetScrollOffset_.x = std::clamp(targetScrollOffset_.x, 0.0f, maxScrollX);
  targetScrollOffset_.y = std::clamp(targetScrollOffset_.y, 0.0f, maxScrollY);

  update();
}

void ScrollViewRenderObject::paint(SkCanvas* canvas) noexcept {
  if (this->children_.empty()) return;
  const auto& child = this->children_.front();
  if (!child) return;

  const auto& bounds_ = getBounds();
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);
  canvas->clipRect(SkRect::MakeWH(bounds_.width, bounds_.height));

  canvas->translate(-scrollOffset_.x, -scrollOffset_.y);
  child->paint(canvas);

  canvas->restore();
  RenderObject::paint(canvas);
}
