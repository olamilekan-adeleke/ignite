#pragma once

#include <memory>
#include <sstream>

#include "component/state_base_component.hpp"
#include "foundation/foundation.hpp"
#include "render/render_object.hpp"

struct OpacityParams {
  float opacity = 1.0f;
};

class OpacityRenderObject : public RenderObject {
 public:
  OpacityRenderObject(const OpacityParams& params = {}) : params_(params) {}

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas* canvas) noexcept override;

  void update(const OpacityParams& params) noexcept { params_ = std::move(params); }

 private:
  OpacityParams params_;
};

class OpacityComponent : public StatelessComponent {
 public:
  OpacityComponent(const OpacityParams& param = {}, const UIKey& key = {}) : param_(param), StatelessComponent(key) {};

  OpacityComponent(const ComponentPtr child, const OpacityParams& param = {}, const UIKey& key = {})
      : param_(param), StatelessComponent(key) {
    if (child) addChild(child);
  };

 protected:
  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<OpacityRenderObject>(param_); }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<OpacityRenderObject>(ro)->update(param_);
  }

  ComponentPtr build() override { return getChild(); }

  OpacityParams param_;

  void debugFillProperties(std::ostringstream& os, int indent) const noexcept override {
    StatelessComponent::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "opacity: " << param_.opacity << "\n";
  };
};
