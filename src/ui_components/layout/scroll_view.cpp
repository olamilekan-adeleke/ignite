#include "layout/scroll_view.hpp"

#include <fmt/base.h>

#include <algorithm>
#include <limits>

#include "basic/ui_component.hpp"
#include "size.hpp"

UISize ScrollView::getIntrinsicSize(UIConstraints constraints) noexcept {
  if (!params_.child) return {0, 0};

  const auto& childSize = params_.child->getIntrinsicSize(constraints);

  UISize preferred{0, 0};
  switch (params_.direction) {
    case ScrollDirection::Vertical:
      preferred.width = std::min(childSize.width, constraints.minWidth);
      preferred.height = std::min(childSize.height, constraints.minHeight);
      break;
    case ScrollDirection::Horizontal:
      preferred.height = std::min(childSize.height, constraints.minHeight);
      preferred.width = constraints.minWidth;
      break;
  }

  return preferred;
}

const std::vector<std::shared_ptr<UIComponent>>& ScrollView::children() const {
  if (params_.child) {
    static const std::vector<std::shared_ptr<UIComponent>> children{params_.child};
    return children;
  }

  return UIComponent::children();
}

void ScrollView::layout(UISize size) {
  setSize(size.width, size.height);
  if (params_.child) {
    params_.child->setPosition(bounds_.x, bounds_.y);
    if (params_.direction == ScrollDirection::Vertical) {
      params_.child->layout({size.width, std::numeric_limits<float>::infinity()});
    } else {
      params_.child->layout({std::numeric_limits<float>::infinity(), size.height});
    }

    const auto& childSize = params_.child->getBounds();
    float maxScrollX = std::max(0.0f, childSize.width - size.width);
    float maxScrollY = std::max(0.0f, childSize.height - size.height);

    // Clamp both current and target
    scrollOffset_.x = std::clamp(scrollOffset_.x, 0.0f, maxScrollX);
    scrollOffset_.y = std::clamp(scrollOffset_.y, 0.0f, maxScrollY);
    targetScrollOffset_.x = std::clamp(targetScrollOffset_.x, 0.0f, maxScrollX);
    targetScrollOffset_.y = std::clamp(targetScrollOffset_.y, 0.0f, maxScrollY);

    update();
  }
}

void ScrollView::draw(SkCanvas* canvas) {
  if (!params_.child) return;

  canvas->save();
  canvas->clipRect(bounds_.toSkRect());

  canvas->translate(-scrollOffset_.x, -scrollOffset_.y);

  params_.child->draw(canvas);
  canvas->restore();

  UIComponent::draw(canvas);
}
