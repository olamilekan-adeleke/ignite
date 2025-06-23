#include "ui_sized.hpp"
#include <fmt/base.h>

Sized::Sized(const SizedParam &param) : size_(param.size), align_(param.align), child_(param.child) {}

void Sized::layout(float parentWidth, float parentHeight) {
  float width = size_.width >= 0 ? size_.width : parentWidth;
  float height = size_.height >= 0 ? size_.height : parentHeight;

  if (child_) {
    child_->layout(width, height);

    auto [x, y] = computeAlignedPosition(align_, width, height, child_->getBounds().width, child_->getBounds().height);
    child_->setPosition(x, y);
  }

  bounds_.width = width;
  bounds_.height = height;
}

void Sized::draw(SkCanvas *canvas) {
  if (child_) {
    canvas->save();
    canvas->translate(bounds_.x, bounds_.y);
    child_->draw(canvas);
    canvas->restore();
  }

  UIComponent::draw(canvas);
}
