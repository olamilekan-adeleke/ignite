#include <fmt/base.h>

#include "component/state_base_component.hpp"
#include "foundation/geometry/rect.hpp"
#include "render/render_object.hpp"

class RenderViewport : public RenderObject {
 public:
  RenderViewport() = default;
  ~RenderViewport() = default;

  void performLayout(UIConstraints constraints) noexcept override {
    for (auto& child : getChildren()) {
      if (child) child->performLayout(constraints);
    }
    setSize({constraints.maxWidth, constraints.maxHeight});
  }

  void paint(SkCanvas* canvas) noexcept override {
    canvas->save();
    for (auto& child : getChildren()) {
      if (child) child->paint(canvas);
    }
    canvas->restore();
  }
};

class AppHost : public StatelessComponent {
 public:
  ComponentPtr child;
  AppHost(ComponentPtr c) : child(std::move(c)), StatelessComponent() {}

  ComponentPtr build() override { return child; }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<RenderViewport>(); }
};
