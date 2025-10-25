#pragma once

#include "component/component.hpp"
#include "foundation/foundation.hpp"
#include "ui_element/state_base_element.hpp"

struct SeparatorParams {
  Axis axis = Axis::VERTICAL;
  float thinkness = 1.0f;
  Color backgroundColor = Color::Gray();
};

class SeparatorRenderObject : public RenderObject {
 public:
  SeparatorRenderObject(const SeparatorParams& params) : params_(params) {}

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas* canvas) noexcept override;

  void update(const SeparatorParams& params) noexcept { params_ = std::move(params); }

 private:
  SeparatorParams params_;
};

class SeparatorComponent : public Component {
 public:
  SeparatorComponent(const SeparatorParams& param = {}, const UIKey key = {}) : params_(param), Component(key) {}

  UIElementPtr createElement() noexcept override { return std::make_shared<LeafUIElement>(shared_from_this()); }

  RenderObjectPtr createRenderObject() const noexcept override {
    return std::make_shared<SeparatorRenderObject>(params_);
  }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<SeparatorRenderObject>(ro)->update(params_);
  }

 protected:
  void debugFillProperties(std::ostringstream& os, int indent) const noexcept override {
    Component::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "axis: " << params_.axis << "\n";
  }

 private:
  SeparatorParams params_;
};
