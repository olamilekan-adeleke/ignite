#include "ui_components/column_component.hpp"

#include <chrono>
#include <cmath>
#include <memory>
#include <utility>

#include "ui_component.hpp"
#include "ui_edge_insets.hpp"

void Column::addChild(std::shared_ptr<UIComponent> child) { children_.push_back(std::move(child)); }

Column::Column(const ColumnParams &params)
    : spacing_(params.spacing),
      cross_axis_alignment_(params.crossAxisAlignment),
      main_axis_alignment_(params.mainAxisAlignment) {
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

void Column::layout(float parentWidth, float parentHeight) {
  bounds_.width = parentWidth;

  float maxChildWidth = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];

    child->layout(parentWidth, 0);

    maxChildWidth = std::fmax(maxChildWidth, child->getBounds().width);
  }

  bounds_.width = maxChildWidth;

  float currentY = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    float crossAxisPosistion = getXPosition(cross_axis_alignment_, bounds_.width, child->getBounds().width);
    child->setPosition(crossAxisPosistion, currentY);

    currentY += child->getBounds().height;
    if (index + 1 != children_.size()) {
      currentY += spacing_;
    }
  }
  bounds_.height = currentY;

  // bounds_.width = parentWidth;
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
