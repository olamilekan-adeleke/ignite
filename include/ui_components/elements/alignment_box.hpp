#include <fmt/base.h>

#include <utility>

#include "component/component.hpp"
#include "component/state_base_component.hpp"
#include "foundation/geometry/ui_alignment.hpp"
#include "render/render_object.hpp"

class AlignmentBoxRenderer : public RenderObject {
 public:
  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas* canvas) noexcept override;
};

class AlignmentBox : public StatelessComponent {
 public:
  AlignmentBox(ComponentPtr child, const UIKey& key = AUTO_KEY) : child_(std::move(child)), StatelessComponent(key) {}

  ComponentPtr build() override { return child_; }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<AlignmentBoxRenderer>(); }

  void debugFillProperties(std::ostringstream& os, int indent) const noexcept override {
    StatelessComponent::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "child: " << typeid(*child_).name() << "\n";
    os << pad << "alignment: " << UIAlignment::Center << "\n";
  }

 private:
  ComponentPtr child_;
};
