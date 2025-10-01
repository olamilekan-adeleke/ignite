#include <fmt/base.h>
#include <memory>
#include <sstream>
#include "size.hpp"
#include "interactive_components/stateful_component.hpp"

void StatefulComponent::markDirty() { isDirty_ = true; }

UISize StatefulComponent::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize size{0, 0};

  const auto child = getChild();
  if (child) size = child->getIntrinsicSize(constraints);
  return size;
}

std::shared_ptr<UIComponent> StatefulComponent::getChild() {
  if (isDirty_ || !cachedBody_) {
    fmt::println("building cached body");
    cachedBody_ = body();
    isDirty_ = false;
  }
  return cachedBody_;
}

void StatefulComponent::layout(UISize size) {
  const auto child = getChild();
  if (child) {
    child->layout({size.width, size.height});
    const auto childBounds = child->getBounds();
    bounds_.width = childBounds.width;
    bounds_.height = childBounds.height;
  }
}

void StatefulComponent::draw(SkCanvas *canvas) {
  auto child = getChild();
  if (child) {
    SkAutoCanvasRestore acr(canvas, true);
    canvas->translate(bounds_.x, bounds_.y);
    child->draw(canvas);
  }
}

const std::vector<std::shared_ptr<UIComponent>> &StatefulComponent::children() const {
  statefulChildren_.clear();
  if (cachedBody_) statefulChildren_.push_back(cachedBody_);
  return statefulChildren_;
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
