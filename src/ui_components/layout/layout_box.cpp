#include "layout/layout_box.hpp"

#include <fmt/base.h>

#include <algorithm>
#include <cassert>

#include "rect.hpp"
#include "size.hpp"

const std::vector<std::shared_ptr<UIComponent>> &LayoutBox::children() const { return params_.children; }

inline float LayoutBox::getMainAxisSize(const UISizing &size) const noexcept {
  switch (params_.axis) {
    case Axis::HORIZONTAL:
      return size.width;
    case Axis::VERTICAL:
      return size.height;
  }
  assert(false);
}

inline float LayoutBox::getCrossAxisSize(const UISizing &size) const noexcept {
  switch (params_.axis) {
    case Axis::HORIZONTAL:
      return size.height;
    case Axis::VERTICAL:
      return size.width;
  }
  assert(false);
}

void LayoutBox::layout(UIConstraints constraints) {
  if (params_.children.empty()) {
    setSize(constraints.minWidth, constraints.minHeight);
    return;
  }

  // fmt::println("FixedBox constraints: minW={} maxW={} minH={} maxH={}",
  //              constraints.minWidth,
  //              constraints.maxWidth,
  //              constraints.minHeight,
  //              constraints.maxHeight);
  const float totalSpacing = params_.childGap * (params_.children.size() - 1);
  float usedMainAxis = 0;
  float maxChildCrossAxis = 0;

  for (size_t i = 0; i < params_.children.size(); ++i) {
    auto &child = params_.children[i];
    UIConstraints childConstraints{};
    if (params_.axis == Axis::HORIZONTAL) {
      childConstraints = UIConstraints::maxSize(constraints.maxWidth - usedMainAxis, constraints.maxHeight);
    } else if (params_.axis == Axis::VERTICAL) {
      childConstraints = UIConstraints::maxSize(constraints.maxWidth, constraints.maxHeight - usedMainAxis);
    }

    child->layout(childConstraints);
    UISizing childSize = child->getSize();
    usedMainAxis += getMainAxisSize(childSize) + (i + 1 == params_.children.size() ? 0 : params_.childGap);
    maxChildCrossAxis = std::max(maxChildCrossAxis, getCrossAxisSize(childSize));
  }

  // Position children
  float mainAxisStartPosition = 0;
  float spacing = params_.childGap;
  for (size_t i = 0; i < params_.children.size(); ++i) {
    const auto &child = params_.children[i];
    const auto &childSize = child->getSize();
    const float mainAdvance = getMainAxisSize(childSize);
    const float crossAxisPosition = 0;

    if (params_.axis == Axis::HORIZONTAL) {
      child->setPosition(mainAxisStartPosition, crossAxisPosition);
    } else if (params_.axis == Axis::VERTICAL) {
      child->setPosition(crossAxisPosition, mainAxisStartPosition);
    }

    mainAxisStartPosition += mainAdvance;
    if (i + 1 != params_.children.size()) mainAxisStartPosition += spacing;
  }

  // Set size for Layout Box
  float totalMainAxisSize = 0;
  float totalCrossAxisSize = 0;
  for (auto &child : params_.children) {
    const auto &childSize = child->getSize();
    totalMainAxisSize += getMainAxisSize(childSize);
    totalCrossAxisSize = std::max(totalCrossAxisSize, getCrossAxisSize(childSize));
  }
  totalMainAxisSize += totalSpacing;

  const auto &[minMain, maxMain] = constraints.mainAxisSize(params_.axis);
  const auto &[minCross, maxCross] = constraints.crossAxisSize(params_.axis);
  const float finalMainAxisSize = std::clamp(totalMainAxisSize, minMain, maxMain);
  const float finalCrossAxisSize = std::clamp(totalCrossAxisSize, minCross, maxCross);
  if (params_.axis == Axis::HORIZONTAL) {
    setSize(finalMainAxisSize, finalCrossAxisSize);
  } else if (params_.axis == Axis::VERTICAL) {
    setSize(finalCrossAxisSize, finalMainAxisSize);
  }

  // fmt::println(
  //     "LayoutBox size: {} {} from {} {}", finalMainAxisSize, finalCrossAxisSize, totalMainAxisSize,
  //     totalCrossAxisSize);
}

void LayoutBox::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  SkRect clipRect = SkRect::MakeWH(bounds_.width, bounds_.height);
  canvas->clipRect(clipRect, SkClipOp::kIntersect);

  for (size_t i = 0; i < params_.children.size(); ++i) {
    auto &child = params_.children[i];
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}
