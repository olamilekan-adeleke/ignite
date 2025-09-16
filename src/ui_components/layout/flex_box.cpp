#include "layout/flex_box.hpp"
#include <_strings.h>
#include <fmt/base.h>
#include <fmt/format.h>
#include <include/core/SkCanvas.h>
#include <cassert>
#include <memory>
#include <vector>
#include "rect.hpp"
#include "size.hpp"
#include "debug/debug_assert.hpp"
#include "ui_component.hpp"

void FlexBox::addChild(std::shared_ptr<UIComponent> child) { children_.push_back(std::move(child)); }

FlexBox::FlexBox(const FlexParam& param) : param_(param), children_(param.children) {
  children_.clear();
  for (auto child : param.children) {
    addChild(child);
  }
}

bool FlexBox::wantsToFillCrossAxis() const { return false; }

bool FlexBox::wantsToFillMainAxis() const { return false; }

const std::vector<std::shared_ptr<UIComponent>>& FlexBox::children() const { return children_; }

void FlexBox::layout(UISize size) {
  if (children_.empty()) {
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

  const bool isHorizontal = param_.flexAxis == Axis::HORIZONTAL;

  // We pass in the parent size to allow child to size it self freely
  const UISize childSize = size;
  for (auto& child : children_) {
    child->layout(childSize);

    const float childMainAxis = getChildMainAxisSize(child->getBounds());
    const float childCrossAxisSize = getChildCrossAxisSize(child->getBounds());

    maxChildMainAxisSize = std::fmax(maxChildMainAxisSize, childMainAxis);
    maxChildCrossAxisSize = std::fmax(maxChildCrossAxisSize, childCrossAxisSize);

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
  const float totalSpacing = children_.size() > 1 ? param_.spacing * (children_.size()) : 0;
  const float availableFillSpace = std::fmax(0, size.height - totalSpacing - fittedMainSize);

  VERIFY(availableFillSpace <= 0, fmt::format("FlexBox::layout: availableFillSpace <= 0"));

  float sizePerChild = flexibleChildCount > 0 ? availableFillSpace / flexibleChildCount : 0;

  // Set the flex box main axis size
  bounds_ = setMainAxisSize(maxChildMainAxisSize, bounds_);
  float totalContentSize = totalSpacing + fittedMainSize;

  // Second pass: this is to size the flexible children.
  for (auto& child : children_) {
    if (child->wantsToFillMainAxis()) {
      child->layout(childSize);
      totalContentSize += getChildCrossAxisSize(child->getBounds());
    }
  }

  // set the flex cross axis size
  bounds_ = setCrossAxisSize(std::fmax(totalContentSize, maxChildCrossAxisSize), bounds_);
  float availableSpacingSize = bounds_.height - totalContentSize;

  // Third pass: this pass is to position all the children
  float startY = bounds_.y;
  float dynmicSpacing = param_.spacing;  // will expand later
  float currentY = startY;
  for (size_t i = 0; i < children_.size(); i++) {
    auto& child = children_[i];
    float crossAxisPosition = 0.0f;
    child->setPosition(crossAxisPosition, currentY);

    currentY += getChildMainAxisSize(child->getBounds());
    if (i + 1 != children_.size()) {
      currentY += dynmicSpacing;
    }
  }
}

void FlexBox::draw(SkCanvas* canvas) {
  canvas->save();

  canvas->translate(bounds_.x, bounds_.y);
  for (auto& child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}
