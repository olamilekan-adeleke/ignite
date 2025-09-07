#include "row_component.hpp"
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
}

void Row::layout(UISize size) {
  float maxChildHeight = 0.0f;
  float totalChildWidth = 0.0f;

  // First pass: layout children and calculate dimensions
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    child->layout({0, size.height});
    maxChildHeight = std::fmax(maxChildHeight, child->getBounds().height);
    totalChildWidth += child->getBounds().width;
  }

  bounds_.height = maxChildHeight;

  // Calculate spacing based on main axis alignment
  float currentX = 0.0f;
  float spaceBetween = 0.0f;

  switch (mainAxisAlignment_) {
    case MainAxisAlignment::START:
      currentX = 0.0f;
      break;
    case MainAxisAlignment::SPACE_BETWEEN:
      if (children_.size() > 1) {
        spaceBetween = (size.width - totalChildWidth) / (children_.size() - 1);
      }
      currentX = 0.0f;
      break;
  }

  // Second pass: position children
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    auto childH = child->getBounds().height;
    auto crossAxisPosition = getYPosition(crossAxisAlignment_, bounds_.height, childH);

    child->setPosition(currentX, crossAxisPosition);
    currentX += child->getBounds().width;

    // Add spacing
    if (mainAxisAlignment_ == MainAxisAlignment::START && spacing_ > 0 && index + 1 != children_.size()) {
      currentX += spacing_;
    } else if (mainAxisAlignment_ == MainAxisAlignment::SPACE_BETWEEN && index + 1 != children_.size()) {
      currentX += spaceBetween;
    }
  }

  // Set final width based on alignment
  if (mainAxisAlignment_ == MainAxisAlignment::SPACE_BETWEEN && !children_.empty()) {
    bounds_.width = size.width;
  } else {
    bounds_.width = currentX;
  }
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
