#include "ui_components/row_component.hpp"

#include <memory>

#include "ui_component.hpp"

void Row::addChild(std::shared_ptr<UIComponent> child) {
  children_.push_back(std::move(child));
}

Row::Row(const RowParams &param) : spacing_(param.spacing) {
  for (const auto c : param.children) {
    addChild(c);
  }
}

void Row::layout(float parentWidth, float parentHeight) {
  float currentX = 0.0f;
  float maxChildHeight = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    child->layout(0, parentHeight);
    child->setPosition(currentX, 0);

    currentX += child->getBounds().width;
    maxChildHeight = std::fmax(maxChildHeight, child->getBounds().height);

    if (spacing_ > 0 && index + 1 != children_.size()) {
      currentX += spacing_;
    }
  }

  // TODO: Add check for overflow later sha
  bounds_.width = currentX;
  bounds_.height = maxChildHeight;
}

const std::vector<std::shared_ptr<UIComponent>> &Row::children() const {
  return children_;
}

void Row::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  for (auto &child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}
