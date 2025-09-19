#include "layout/flex_box.hpp"
#include "debug/debug_assert.hpp"
#include <fmt/base.h>
#include <include/core/SkCanvas.h>
#include <cmath>
#include <memory>
#include <vector>
#include "rect.hpp"
#include "size.hpp"

void FlexBox::addChild(std::shared_ptr<UIComponent> child) { param_.children.push_back(std::move(child)); }

FlexBox::FlexBox(const FlexParam& param) : param_(param) {}

bool FlexBox::wantsToFillCrossAxis() const { return false; }

bool FlexBox::wantsToFillMainAxis() const { return false; }

const std::vector<std::shared_ptr<UIComponent>>& FlexBox::children() const { return param_.children; }

UISize FlexBox::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize size;
  for (auto& child : param_.children) {
    const auto childSize = child->getIntrinsicSize(constraints);
    const float totalSpacing = param_.children.size() > 0 ? param_.spacing * (param_.children.size() - 1) : 0;
    if (param_.axis == Axis::HORIZONTAL) {
      size = childSize.combineHorizontal(size);
      size = UISize{.width = size.width + totalSpacing, .height = size.height};
    } else if (param_.axis == Axis::VERTICAL) {
      size = childSize.combineVertical(size);
      size = UISize{.width = size.width, .height = size.height + totalSpacing};
    }
  }
  return size;
}

void FlexBox::layout(UISize size) {
  if (param_.children.empty()) {
    bounds_ = bounds_.copyWith({.width = 0, .height = 0});
    return;
  }

  // First pass: this pass transverse the children to get the size of the children,
  // basically we want to known the main axis size for each child. Also note that
  // there are two possible type of children, one with fit size and others that
  // are allowed to grow/flex. The second pass will account for those that want to flex

  // Main Axis Size Of Children With Fitted Size
  float fittedMainSize = 0.0f;
  float maxChildMainAxisSize = 0.0f;
  float maxChildCrossAxisSize = 0.0f;
  int flexibleChildCount = 0;

  const bool isHorizontal = param_.axis == Axis::HORIZONTAL;

  // We pass in the parent size to allow child to size it self freely
  for (auto& child : param_.children) {
    auto childIntrinsicSize = child->getIntrinsicSize(size.toUIConstraints());
    child->layout(childIntrinsicSize);

    const float childMainAxis = getChildMainAxisSize(child->getBounds());
    const float childCrossAxisSize = getChildCrossAxisSize(child->getBounds());

    maxChildMainAxisSize = std::max(maxChildMainAxisSize, childMainAxis);
    maxChildCrossAxisSize = std::max(maxChildCrossAxisSize, childCrossAxisSize);

    const bool wantToExpand = child->wantsToFillMainAxis();
    if (wantToExpand) {
      flexibleChildCount++;
    } else {
      fittedMainSize += childMainAxis;
    }
  }

  // Before the second pass, we need to get what available space left, this will
  // be a sum of the total main axis size of children with fitted size [fittedMainSize]
  // and the total spacing. Once we know that we can then known the amount of space
  // to share among the flexible children, if any
  const float totalSpacing = param_.children.size() > 1 ? param_.spacing * (param_.children.size() - 1) : 0;
  const float availableMainAxisSize = param_.axis == Axis::HORIZONTAL ? size.width : size.height;
  const float availableCrossAxisSize = param_.axis == Axis::HORIZONTAL ? size.height : size.width;
  const float availableFillSpace = std::fmax(0, (availableMainAxisSize - fittedMainSize - totalSpacing));

  VERIFY(availableFillSpace == 0,
         fmt::format("FlexBox::layout [{:?}:{}] [{} child] [totalSpacing: {}, fittedMainSize: {}, "
                     "availableMainAxisSize: {}, availableFillSpace: {}]",
                     axisToString(param_.axis),
                     size.height,
                     param_.children.size(),
                     totalSpacing,
                     fittedMainSize,
                     availableMainAxisSize,
                     availableFillSpace));

  const float sizePerChild = flexibleChildCount > 0 ? availableFillSpace / flexibleChildCount : 0;

  // Set the flex box main axis size
  const float totalContentSize = totalSpacing + fittedMainSize + (sizePerChild * flexibleChildCount);
  bounds_ = setMainAxisSize(totalContentSize, bounds_, size);

  // Second pass: this is to size the flexible children.
  for (auto& child : param_.children) {
    if (child->wantsToFillMainAxis()) {
      UISize sizedChild = isHorizontal ? UISize{.width = sizePerChild, .height = size.height}
                                       : UISize{.width = size.width, .height = sizePerChild};
      child->layout(sizedChild);
      maxChildCrossAxisSize = std::max(maxChildCrossAxisSize, getChildCrossAxisSize(child->getBounds()));
    }
  }

  // set the flex cross axis size
  bounds_ = setCrossAxisSize(maxChildCrossAxisSize, bounds_);

  // Third pass: position children along main axis
  const auto availableSpaceLeft = std::max(0.0f, availableMainAxisSize - totalContentSize);
  float currentMain = getMainAxisStartPosition(availableSpaceLeft);
  const float spacing = getMainAxisSpacing(availableSpaceLeft);
  for (size_t i = 0; i < param_.children.size(); ++i) {
    auto& child = param_.children[i];
    auto childBound = child->getBounds();
    const float mainAdvance = getChildMainAxisSize(childBound);
    const float crossPos = getCrossAxisStartPosition(availableCrossAxisSize, getChildCrossAxisSize(childBound));

    if (isHorizontal) {
      child->setPosition(currentMain, crossPos);
    } else {
      child->setPosition(crossPos, currentMain);
    }
    currentMain += mainAdvance;
    if (i + 1 != param_.children.size()) currentMain += spacing;
  }
}

void FlexBox::draw(SkCanvas* canvas) {
  canvas->save();

  canvas->translate(bounds_.x, bounds_.y);

  // Clip to the Flex Box bounds to prevent children from drawing outside
  SkRect clipRect = SkRect::MakeWH(bounds_.width, bounds_.height);
  canvas->clipRect(clipRect, SkClipOp::kIntersect);

  for (auto& child : param_.children) {
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}
