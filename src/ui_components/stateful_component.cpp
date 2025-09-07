#include <fmt/base.h>
#include <memory>
#include "size.hpp"
#include "stateful_component.hpp"

void StatefulComponent::layout(UISize size) {
  const auto child = getChild();
  if (child) {
    child->layout({size.width, size.height});
    bounds_ = child->getBounds();
  }
}

void StatefulComponent::draw(SkCanvas *canvas) {
  auto child = getChild();
  // fmt::println("StatefulComponent child: {}", child ? std::to_string(child->getBounds().height) : "Null");
  if (child) {
    child->draw(canvas);
  }

  isDirty_ = false;
  needToRedraw_ = false;
}

bool StatefulComponent::isDirty() const { return isDirty_; }

std::shared_ptr<UIComponent> StatefulComponent::getChild() {
  if (isDirty_ || needToRedraw_ || !cachedBody_) {
    cachedBody_ = body();
    // needToRedraw_ = false;
    // isDirty_ = false;
  }
  return cachedBody_;
}

const std::vector<std::shared_ptr<UIComponent>> &StatefulComponent::children() const {
  static std::vector<std::shared_ptr<UIComponent>> statefulChildren;
  statefulChildren.clear();
  if (cachedBody_) {
    statefulChildren.push_back(cachedBody_);
  }
  return statefulChildren;
}

void StatefulComponent::markDirty() {
  if (isDirty_ == false) {
    isDirty_ = true;
    needToRedraw_ = true;
    setKey(UIKey());
  }
}

bool StatefulComponent::processChildTaps(const UITapEvent &event) {
  const auto child = getChild();
  if (child) {
    return child->processTap(event);
  }
  return false;
}
