#include "ui_components/row_component.hpp"
#include <fmt/base.h>

#include <memory>

#include "ui_component.hpp"

void Row::addChild(std::shared_ptr<UIComponent> child) { children_.push_back(std::move(child)); }

Row::Row(const RowParams &param)
    : spacing_(param.spacing),
      mainAxisAlignment_(param.mainAxisAlignment),
      crossAxisAlignment_(param.crossAxisAlignment) {
  for (const auto c : param.children) {
    addChild(c);
  }
}

inline float getYPosition(CrossAxisAlignment axis, float height, float childH) {
  switch (axis) {
    case CrossAxisAlignment::START:
      return 0.0f;
    case CrossAxisAlignment::CENTER:
      return (height - childH) / 2.0f;
    case CrossAxisAlignment::END:
      return height - childH;
  }

  return 0.0f;
}

void Row::layout(float parentWidth, float parentHeight) {
  float maxChildHeight = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    child->layout(0, parentHeight);
    maxChildHeight = std::fmax(maxChildHeight, child->getBounds().height);
  }
  bounds_.height = maxChildHeight;

  float currentX = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];

    auto childH = child->getBounds().height;
    auto crossAxisPosition = getYPosition(crossAxisAlignment_, bounds_.height, childH);
    child->setPosition(currentX, crossAxisPosition);

    currentX += child->getBounds().width;
    if (spacing_ > 0 && index + 1 != children_.size()) {
      currentX += spacing_;
    }
  }
  bounds_.width = currentX;
}

const std::vector<std::shared_ptr<UIComponent>> &Row::children() const { return children_; }

void Row::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  for (auto &child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}
