#include "ui_components/column_component.hpp"

#include <memory>

#include "ui_component.hpp"

Column& Column::addChild(std::shared_ptr<UIComponent> child) {
  children_.push_back(std::move(child));
  return *this;
}

template <typename... Children>
Column::Column(std::shared_ptr<UIComponent> first, Children... rest) {
  children_.reserve(sizeof...(rest) + 1);

  addChild(std::move(first));
  (addChild(std::move(rest)), ...);
}

void Column::layout(float parentWidth, float parentHeight) {
  bounds_.width = parentWidth;
  bounds_.height = parentHeight;

  float currentY = 0.0f;
  for (auto& child : children_) {
    child->layout(parentWidth, parentHeight);
    child->setPosistion(0, currentY);

    currentY += child->getBounds().height;
    bounds_.height += child->getBounds().height;
  }
}

void Column::draw(SkCanvas* canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  for (auto& child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
}
