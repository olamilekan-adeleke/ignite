#include <fmt/base.h>
#include <memory>
#include <sstream>
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
  statefulChildren_.clear();
  if (cachedBody_) statefulChildren_.push_back(cachedBody_);
  return statefulChildren_;
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

std::string StatefulComponent::toString(int indent) const {
  std::ostringstream os;
  std::string pad(indent, ' ');

  os << pad << typeid(*this).name() << " {\n";
  debugFillProperties(os, indent);

  auto kids = children();
  if (!kids.empty()) {
    os << pad << "  children: [\n";
    for (const auto &child : kids) {
      os << child->toString(indent + 4) << "\n";
    }
    os << pad << "  ]\n";
  }

  os << pad << "}";
  return os.str();
}
