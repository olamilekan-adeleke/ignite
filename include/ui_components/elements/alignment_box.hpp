#include <fmt/base.h>

#include <utility>

#include "component/component.hpp"
#include "component/state_base_component.hpp"
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

 private:
  ComponentPtr child_;
};
