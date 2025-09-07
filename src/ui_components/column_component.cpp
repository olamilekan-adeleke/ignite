#include <fmt/base.h>

#include "column_component.hpp"
#include <cmath>
#include <memory>
#include <utility>

#include "ui_alignment.hpp"
#include "ui_component.hpp"

void Column::addChild(std::shared_ptr<UIComponent> child) { children_.push_back(std::move(child)); }

Column::Column(const ColumnParams &params)
    : spacing_(params.spacing),
      crossAxisAlignment_(params.crossAxisAlignment),
      mainAxisAlignment_(params.mainAxisAlignment),
      crossAxisSize_(params.crossAxisSize),
      mainAxisSize_(params.mainAxisSize) {
  for (const auto &c : params.children) {
    addChild(c);
  }
}

bool Column::wantsToFillMainAxis() const { return mainAxisSize_ == MainAxisSize::FILL; }

bool Column::wantsToFillCrossAxis() const { return crossAxisSize_ == CrossAxisSize::FILL; }

void Column::layout(UISize size) {
  if (children_.empty()) {
    bounds_.height = 0;
    bounds_.width = 0;
    return;
  }

  float maxChildWidth = 0.0f;
  float totalHeightOfChildrenWithFittedSize = 0.0f;
  int fillChildrenCount = 0;

  // first pass:
  for (auto &child : children_) {
    bool childWantToExpand = child->wantsToFillMainAxis();

    child->layout({size.width, 0});  // use 0 as height to just get the child width
    maxChildWidth = std::fmax(maxChildWidth, child->getBounds().width);

    if (childWantToExpand) {
      fillChildrenCount++;
    } else {
      totalHeightOfChildrenWithFittedSize += child->getBounds().height;
    }
  }

  float totalSpacing = children_.size() > 1 ? spacing_ * (children_.size() - 1) : 0;

  float avaliableHeightSpaceLeftForFill =
      std::fmax(0.0f, size.height - totalHeightOfChildrenWithFittedSize - totalSpacing);
  float heightPerFillChild = (fillChildrenCount > 0) ? avaliableHeightSpaceLeftForFill / fillChildrenCount : 0.0f;

  if (crossAxisSize_ == CrossAxisSize::FILL) {
    bounds_.width = size.width;
  } else {
    bounds_.width = maxChildWidth;
  }

  // Second pass: final layout for expanding children
  float totalContentHeight = totalHeightOfChildrenWithFittedSize + totalSpacing;
  for (auto &child : children_) {
    if (child->wantsToFillMainAxis()) {
      child->layout({bounds_.width, heightPerFillChild});
      totalContentHeight += child->getBounds().height;
    }
  }

  // set the column height now since will know that
  if (mainAxisSize_ == MainAxisSize::FILL) {
    bounds_.height = size.height;
  } else {
    bounds_.height = totalContentHeight;
  }

  float availableSpace = bounds_.height - totalContentHeight;

  float startY = getMainAxisStartPosition(mainAxisAlignment_, availableSpace, totalContentHeight, children_.size());
  float dynamicSpacing = getSpaceBetweenChildren(mainAxisAlignment_, availableSpace, children_.size(), spacing_);

  // Third pass: Position all children
  float currentY = startY;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];

    float crossAxisPosition = getXPosition(crossAxisAlignment_, bounds_.width, child->getBounds().width);
    child->setPosition(crossAxisPosition, currentY);

    currentY += child->getBounds().height;

    if (index + 1 != children_.size()) {
      currentY += dynamicSpacing;
    }
  }
}

void Column::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  for (auto &child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}

const std::vector<std::shared_ptr<UIComponent>> &Column::children() const { return children_; }
