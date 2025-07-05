#include "ui_components/column_component.hpp"
#include <fmt/base.h>

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

inline float getXPosition(CrossAxisAlignment axis, float parentWidth, float childWidth) {
  switch (axis) {
    case CrossAxisAlignment::START:
      return 0;
    case CrossAxisAlignment::CENTER:
      return (parentWidth - childWidth) / 2.0f;
    case CrossAxisAlignment::END:
      return parentWidth - childWidth;
  }
}

bool Column::wantsToFillMainAxis() const { return mainAxisSize_ == MainAxisSize::FILL; }

bool Column::wantsToFillCrossAxis() const { return crossAxisSize_ == CrossAxisSize::FILL; }

void Column::layout(float parentWidth, float parentHeight) {
  if (children_.empty()) {
    bounds_.height = 0;
    bounds_.width = 0;
    return;
  }

  float maxChildWidth = 0.0f;

  float totalHeightOfChildrenWithFittedSize = 0.0f;
  int fillChildrenCount = 0;

  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    bool childWantToExpand = (*child).wantsToFillMainAxis();

    if (childWantToExpand) {
      fillChildrenCount += 1;
      child->layout(parentWidth, 0);  // use 0 as height to just get the child width
      maxChildWidth = std::fmax(maxChildWidth, child->getBounds().width);
    } else {
      child->layout(parentWidth, 0);
      maxChildWidth = std::fmax(maxChildWidth, child->getBounds().width);
      totalHeightOfChildrenWithFittedSize += child->getBounds().height;
    }
  }

  if (children_.size() > 1) {
    totalHeightOfChildrenWithFittedSize += spacing_ * (children_.size() - 1);
  }

  float avaliableHeightSpaceLeftForFill = std::fmax(0.0f, parentHeight - totalHeightOfChildrenWithFittedSize);
  float heightPerFillChild = (fillChildrenCount > 0) ? avaliableHeightSpaceLeftForFill / fillChildrenCount : 0.0f;

  if (crossAxisSize_ == CrossAxisSize::FILL) {
    bounds_.width = parentWidth;
  } else {
    bounds_.width = maxChildWidth;
  }

  // layout all child and position them base on their Cross Axis Alignment
  float currentY = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    bool childWantToExpand = child->wantsToFillMainAxis();

    if (childWantToExpand) {
      child->layout(bounds_.width, heightPerFillChild);
    }

    float crossAxisPosition = getXPosition(crossAxisAlignment_, bounds_.width, child->getBounds().width);
    child->setPosition(crossAxisPosition, currentY);
    currentY += child->getBounds().height;

    if (index + 1 != children_.size()) {
      currentY += spacing_;
    }
  }

  if (mainAxisSize_ == MainAxisSize::FILL) {
    bounds_.height = parentHeight;
  } else {
    bounds_.height = currentY;
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
