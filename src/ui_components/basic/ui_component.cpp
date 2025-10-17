#include "basic/ui_component.hpp"

#include <memory>
#include <sstream>

std::string UIComponent::toString(int indent) const {
  std::ostringstream os;
  std::string pad(indent, ' ');

  os << pad << typeid(*this).name() << " {\n";
  debugFillProperties(os, indent + 1);

  auto kids = children();
  if (!kids.empty()) {
    os << pad << "  children: [\n";
    for (const auto& child : kids) {
      os << child->toString(indent + 4) << "\n";
    }
    os << pad << "  ]\n";
  } else if (kids.empty() && this->getChild()) {
    os << pad << "  child {\n";
    os << this->getChild()->toString(indent + 4) << "\n";
    os << pad << "  }\n";
  }

  os << pad << "}";
  return os.str();
}
