#pragma once
#include <fmt/base.h>

#include "component/state_base_component.hpp"
#include "render/render_object.hpp"

class AlignmentBoxRenderer : public RenderObject {
 public:
  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas* canvas) noexcept override;
};

class AlignmentBox : public StatelessComponent {
 public:
  AlignmentBox(const UIKey& key = {}) : StatelessComponent(key) {}
  AlignmentBox(ComponentPtr child, const UIKey& key = {}) : StatelessComponent(key) {
    if (child) addChild(child);
  }

  ComponentPtr build() override { return getChild(); }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<AlignmentBoxRenderer>(); }

  void debugFillProperties(std::ostringstream& os, int indent) const noexcept override {
    StatelessComponent::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "alignment: " << UIAlignment::Center << "\n";
  }
};
