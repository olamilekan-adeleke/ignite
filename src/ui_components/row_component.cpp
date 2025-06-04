#include "ui_components/row_component.hpp"

#include <memory>

#include "ui_component.hpp"

Row& Row::addChild(std::shared_ptr<UIComponent> child) {
  children_.push_back(std::move(child));
  return *this;
}

template <typename... Children>
Row::Row(std::shared_ptr<UIComponent> first, Children... rest) {
  children_.reserve(sizeof...(rest) + 1);

  addChild(std::move(first));
  (addChild(std::move(rest)), ...);
}

void Row::layout(float parentWidth, float parentHeight) {
  bounds_.width = parentWidth;
  bounds_.height = parentHeight;

  float currentX = 0.0f;
  for (auto& child : children_) {
    child->layout(parentWidth, parentHeight);
    child->setPosistion(currentX, 0);

    currentX += child->getBounds().width;
    bounds_.width += child->getBounds().width;
  }
}

void Row::draw(SkCanvas* canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  for (auto& child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
}
