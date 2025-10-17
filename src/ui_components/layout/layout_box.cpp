#include "layout/layout_box.hpp"

#include <fmt/base.h>
#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "axis.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "ui_alignment.hpp"

const std::vector<std::shared_ptr<UIComponent>> &LayoutBox::children() const { return params_.children; }

inline float LayoutBox::distributeFlexSpace(float availableSize,
                                            uint numChildren,
                                            std::shared_ptr<UIComponent> child) const noexcept {
  return availableSize / numChildren;
}

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

inline float LayoutBox::getCrossAxisPosition(const UISizing &size) const noexcept {
  const auto crossAxisSize = getCrossAxisSize(getSize());
  const auto childCrossAxisSize = getCrossAxisSize(size);

  switch (params_.crossAxisAlignment) {
    case CrossAxisAlignment::START:
      return 0.0f;
    case CrossAxisAlignment::CENTER:
      return (crossAxisSize - childCrossAxisSize) / 2.0f;
    case CrossAxisAlignment::END:
      return crossAxisSize - childCrossAxisSize;
  }
  return 0.0f;
}

void LayoutBox::layout(UIConstraints constraints) {
  if (params_.children.empty()) {
    setSize(constraints.minWidth, constraints.minHeight);
    return;
  }

  // first pass - get size for fitted children
  const float totalSpacing = params_.childGap * (params_.children.size() - 1);
  float usedMainAxis = 0;
  float maxChildCrossAxis = 0;
  uint32_t flexibleChildrenCount = 0;

  for (size_t i = 0; i < params_.children.size(); ++i) {
    auto &child = params_.children[i];

    const auto wantsToFillMainAxis =
        params_.axis == Axis::VERTICAL ? child->wantsToFillMainAxis() : child->wantsToFillCrossAxis();
    if (wantsToFillMainAxis) {
      flexibleChildrenCount += 1;
      continue;
    }

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

  const auto &[minMain, maxMain] = constraints.mainAxisSize(params_.axis);
  const auto &[minCross, maxCross] = constraints.crossAxisSize(params_.axis);

  // second pass for flexible children
  const uint32_t flexibleGapCount = flexibleChildrenCount;
  const float mainAxisSizeLeftAvailable = std::max(0.0f, maxMain - usedMainAxis);
  const auto &[_, crossAxisSizeAvailable] = constraints.crossAxisSize(params_.axis);

  for (size_t i = 0; i < params_.children.size(); ++i) {
    auto &child = params_.children[i];

    const auto wantsToFillMainAxis =
        params_.axis == Axis::VERTICAL ? child->wantsToFillMainAxis() : child->wantsToFillCrossAxis();
    if (!wantsToFillMainAxis) continue;
    // if (!child->wantsToFill()) continue;

    float mainAxisSizePerChild = distributeFlexSpace(mainAxisSizeLeftAvailable, flexibleGapCount, child);

    UIConstraints childConstraints{};
    if (params_.axis == Axis::HORIZONTAL) {
      childConstraints = UIConstraints::maxSize(mainAxisSizePerChild, crossAxisSizeAvailable);
    } else if (params_.axis == Axis::VERTICAL) {
      childConstraints = UIConstraints::maxSize(crossAxisSizeAvailable, mainAxisSizePerChild);
    }

    child->layout(childConstraints);
    UISizing childSize = child->getSize();
  }

  // Set size for Layout Box
  float totalMainAxisSize = 0;
  float totalCrossAxisSize = 0;
  if (params_.sizing == MainAxisSize::FILL) {
    totalMainAxisSize = maxMain;
  }

  for (auto &child : params_.children) {
    const auto &childSize = child->getSize();
    if (params_.sizing == MainAxisSize::FIT) totalMainAxisSize += getMainAxisSize(childSize);
    totalCrossAxisSize = std::max(totalCrossAxisSize, getCrossAxisSize(childSize));
  }
  totalMainAxisSize += totalSpacing;

  const float finalMainAxisSize = std::clamp(totalMainAxisSize, minMain, maxMain);
  const float finalCrossAxisSize = std::clamp(totalCrossAxisSize, minCross, maxCross);
  if (params_.axis == Axis::HORIZONTAL) {
    setSize(finalMainAxisSize, finalCrossAxisSize);
  } else if (params_.axis == Axis::VERTICAL) {
    setSize(finalCrossAxisSize, finalMainAxisSize);
  }

  // Position children
  float mainAxisStartPosition = 0;
  float spacing = params_.childGap;
  const Offset &parentGlobalOffset = getGlobalOffset();

  float containerGlobalX = getGlobalOffset().x;
  float containerGlobalY = getGlobalOffset().y;
  for (size_t i = 0; i < params_.children.size(); ++i) {
    const auto &child = params_.children[i];
    const auto &childSize = child->getSize();
    const float mainAdvance = getMainAxisSize(childSize);
    const float crossAxisPosition = getCrossAxisPosition(childSize);

    float childRelativeX, childRelativeY;
    if (params_.axis == Axis::HORIZONTAL) {
      childRelativeX = mainAxisStartPosition;
      childRelativeY = crossAxisPosition;
    } else if (params_.axis == Axis::VERTICAL) {
      childRelativeX = crossAxisPosition;
      childRelativeY = mainAxisStartPosition;
    }

    child->setPosition(childRelativeX, childRelativeY);
    // child->updateGlobalOffset(parentGlobalOffset);
    child->updateGlobalOffset({
        parentGlobalOffset.x + childRelativeX,
        parentGlobalOffset.y + childRelativeY,
    });
    mainAxisStartPosition += mainAdvance;
    if (i + 1 != params_.children.size()) mainAxisStartPosition += spacing;
  }
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
