#pragma once

#include <include/core/SkCanvas.h>

#include <memory>
#include <sstream>

#include "ui_element/state_base_element.hpp"

struct CheckBoxParams {
  bool checked = false;
  UISizing size{18, 18};
  float radius = 4.0f;
  Color fillColor = Color::Blue();
  Color inactiveBorderColor = Color::Gray();
  Color checkColor = Color::White();
  TapListener onTap = nullptr;
};

class CheckBoxRender : public RenderObject {
 public:
  CheckBoxRender(const CheckBoxParams &params) : params_(params) {
    if (params_.onTap) setTapListener(params_.onTap);
  }

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas *canvas) noexcept override;

  void updateParams(const CheckBoxParams &params) noexcept { params_ = std::move(params); }

  bool processChildTaps(const UITapEvent &event) override {
    const auto bounds_ = getBounds();
    if (params_.onTap == nullptr) return false;

    UITapEvent localEvent = event;
    localEvent.x -= bounds_.x;
    localEvent.y -= bounds_.y;
    const bool result = onTap(localEvent, bounds_);
    return result;
  }

 private:
  CheckBoxParams params_;

  void drawCheckBox(SkCanvas *canvas) const;
};

class UICheckBoxComponent : public Component {
 public:
  UICheckBoxComponent(const CheckBoxParams &params, const UIKey key = {}) : params_(params), Component(key) {}

  UIElementPtr createElement() noexcept override { return std::make_shared<LeafUIElement>(shared_from_this()); }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<CheckBoxRender>(params_); }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<CheckBoxRender>(ro)->updateParams(params_);
  }

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const noexcept override {
    Component::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "enable: " << (params_.checked ? "true" : "false") << "\n";
    os << pad << "fillColor: " << params_.fillColor.toString() << "\n";
    os << pad << "checkColor: " << params_.checkColor.toString() << "\n";
    os << pad << "onTap: " << (params_.onTap ? "true" : "false") << "\n";
  }

 private:
  CheckBoxParams params_;
};
