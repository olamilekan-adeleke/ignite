#pragma once

#include <memory>
#include <sstream>
#include <utility>

#include "component/component.hpp"
#include "component/state_base_component.hpp"
#include "foundation/foundation.hpp"
#include "render/render_object.hpp"

struct FixedBoxParam {
  UISizing size = UISizing::Fixed(0, 0);
  UIAlignment alignment = UIAlignment::Center;
};

class FixedBoxRenderObject : public RenderObject {
 public:
  FixedBoxRenderObject(const FixedBoxParam &params = {}) : params_(params) {}

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas *canvas) noexcept override;

  void update(const FixedBoxParam &params) noexcept { params_ = std::move(params); }

  float computeWidth() const noexcept;
  float computeHeight() const noexcept;

 private:
  FixedBoxParam params_;
};

class FixedBox : public StatelessComponent {
 public:
  FixedBox(const FixedBoxParam &param = {}, const UIKey &key = {})
      : params_(std::move(param)), StatelessComponent(key) {}

  FixedBox(const ComponentPtr child, const FixedBoxParam &param = {}, const UIKey &key = {})
      : params_(std::move(param)), StatelessComponent(key) {
    if (child) addChild(child);
  };

 protected:
  RenderObjectPtr createRenderObject() const noexcept override {
    return std::make_shared<FixedBoxRenderObject>(params_);
  }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<FixedBoxRenderObject>(ro)->update(params_);
  }

  ComponentPtr build() override { return getChild(); }

  bool wantsToFillMainAxis() const noexcept override { return params_.size.isGrowHeight(); }
  bool wantsToFillCrossAxis() const noexcept override { return params_.size.isGrowWidth(); }

  // bool processChildTaps(const UITapEvent &event) override {
  //   if (params_.child) {
  //     UITapEvent localEvent = event;
  //     localEvent.x -= bounds_.x;
  //     localEvent.y -= bounds_.y;
  //
  //     return params_.child->processTap(localEvent);
  //   }
  //   return false;
  // }

  void debugFillProperties(std::ostringstream &os, int indent) const noexcept override {
    StatelessComponent::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "size: " << params_.size << "\n";
    os << pad << "alignment: " << params_.alignment << "\n";

    // if (auto &child = children_.front()) {
    //   os << pad << "hasChild: " << (child ? "true" : "false") << "\n";
    //   os << pad << "type: " << Helper::to_string(child) << "\n";
    // }
  }

 private:
  FixedBoxParam params_;
};
