#include "interactive_components/stateful_component.hpp"

#include <fmt/base.h>

#include <memory>
#include <sstream>

void StatefulComponent::markDirty() { isDirty_ = true; }

std::shared_ptr<UIComponent> StatefulComponent::getChild() {
  if (isDirty_ || !cachedBody_) {
    cachedBody_ = body();
    fmt::println("building cached body for {}", typeid(*cachedBody_).name());
    isDirty_ = false;
  }
  return cachedBody_;
}

void StatefulComponent::layout(UIConstraints size) {
  const auto child = getChild();
  if (child) {
    child->layout(size);
    const auto childBounds = child->getBounds();

    child->setPosition(bounds_.x, bounds_.y);

    bounds_.width = childBounds.width;
    bounds_.height = childBounds.height;

    child->updateGlobalOffset(getGlobalOffset());
  }
}

void StatefulComponent::draw(SkCanvas *canvas) {
  auto child = getChild();
  if (child) {
    SkAutoCanvasRestore acr(canvas, true);
    // canvas->translate(bounds_.x, bounds_.y);
    child->draw(canvas);
  }
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
