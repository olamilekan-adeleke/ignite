#include "ui_components/column_component.hpp"

#include <memory>

#include "ui_component.hpp"

void Column::addChild(std::shared_ptr<UIComponent> child) {
  children_.push_back(std::move(child));
  // return *this;
}

Column::Column(const ColumnParams& params) : spacing_(params.spacing) {
  for (auto& c : params.children) {
    addChild(c);
  }
}

// template <typename... Children>
// Column::Column(std::shared_ptr<UIComponent> first, Children... rest) {
//   children_.reserve(sizeof...(rest) + 1);
//
//   addChild(std::move(first));
//   (addChild(std::move(rest)), ...);
// }

void Column::layout(float parentWidth, float parentHeight) {
  bounds_.width = parentWidth;
  // bounds_.height = parentHeight;

  float currentY = 0.0f;
  // for (auto& child : children_) {
  for (size_t index = 0; index < children_.size(); index++) {
    auto& child = children_[index];

    child->layout(parentWidth, parentHeight);
    child->setPosistion(0, currentY);

    currentY += child->getBounds().height;
    // bounds_.height += child->getBounds().height;

    // add spacing
    if (index + 1 != children_.size()) {
      currentY += spacing_;
    }
    // bounds_.height += spacing_;
  }

  bounds_.height = currentY;
}

void Column::draw(SkCanvas* canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  for (auto& child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}
