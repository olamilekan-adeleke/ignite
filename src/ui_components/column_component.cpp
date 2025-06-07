#include "ui_components/column_component.hpp"

#include <cmath>
#include <memory>

#include "ui_component.hpp"

void Column::addChild(std::shared_ptr<UIComponent> child) {
  children_.push_back(std::move(child));
}

Column::Column(const ColumnParams &params) : spacing_(params.spacing) {
  for (auto &c : params.children) {
    addChild(c);
  }
}

void Column::layout(float parentWidth, float parentHeight) {
  bounds_.width = parentWidth;

  float currentY = 0.0f;
  float maxChildWidth = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];

    child->layout(parentWidth, 0);
    child->setPosition(0, currentY);

    currentY += child->getBounds().height;
    maxChildWidth = std::fmax(maxChildWidth, child->getBounds().width);
    if (index + 1 != children_.size()) {
      currentY += spacing_;
    }
  }

  bounds_.height = currentY;
  bounds_.width = maxChildWidth;
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

std::vector<std::shared_ptr<UIComponent>> Column::children() {
  return children_;
}
