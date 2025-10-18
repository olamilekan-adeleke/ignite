#include "interactive_components/stateful_component.hpp"

#include <fmt/base.h>

#include <memory>
#include <sstream>

void StatefulComponent::markDirty() { isDirty_ = true; }

std::shared_ptr<UIComponent> StatefulComponent::getChild() {
  if (isDirty_ || !cachedBody_) {
    fmt::println("REBUILDING - isDirty: {}, cachedBody null: {}", isDirty_, !cachedBody_);
    cachedBody_ = body();
    fmt::println("building cached body for {}", typeid(*cachedBody_).name());
    isDirty_ = false;
    // invalidateLayout();
  } else {
    // fmt::println("USING CACHE - {}", typeid(*cachedBody_).name());
  }
  return cachedBody_;
}

void StatefulComponent::layout(UIConstraints size) {
  const auto child = getChild();
  if (child) {
    // fmt::println("LAYOUT - {} | size: {}, {}", typeid(*child).name(), size.maxWidth, size.maxWidth);
    child->layout(size);
    const auto &childBounds = child->getSize();
    setSize(childBounds);

    child->setPosition(bounds_.x, bounds_.y);
    // child->setPosition(0, 0);
    // child->updateGlobalOffset(getGlobalOffset());

    // child->setPosition(0, 0);
    // fmt::println(
    //     "setting position to {}, {} | {} -> {}", bounds_.x, bounds_.y, typeid(*child).name(), child->key().value());
    child->updateGlobalOffset(getGlobalOffset());
  }
  // isLayoutValid_ = true;
}

void StatefulComponent::draw(SkCanvas *canvas) {
  if (cachedBody_) {
    // if (!cachedBody_->isLayoutValid()) {

    // cachedBody_->layout(getConstraints()); // Assuming getConstraints() provides the correct constraints
    // }
    canvas->save();
    // canvas->translate(bounds_.x, bounds_.y);
    cachedBody_->draw(canvas);
    canvas->restore();
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
