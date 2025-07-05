#pragma once

#include "color.hpp"
#include "ui_component.hpp"
#include "ui_components/ui_component.hpp"
#include "ui_edge_insets.hpp"

struct ViewParams {
  UIEdgeInsets insets = {0.0f, 0.0f, 0.0f, 0.0f};
  UIEdgeInsets margin = {0.0f, 0.0f, 0.0f, 0.0f};
  Color backgroundColor = Color::Clear();
  float borderRadius = 0.0f;
  bool antiAlias = false;
  bool tappable = true;
  TapListener onTap = nullptr;
  std::shared_ptr<UIComponent> child = nullptr;
};

class View : public UIComponent {
 public:
  View(const ViewParams &params = ViewParams());

  View(const View &) = delete;
  View &operator=(const View &) = delete;

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas *canvas) override;

  bool wantsToFillMainAxis() const override {
    if (params_.child) {
      return params_.child->wantsToFillMainAxis();
    } else {
      return UIComponent::wantsToFillMainAxis();
    }
  }

  bool wantsToFillCrossAxis() const override {
    if (params_.child) {
      return params_.child->wantsToFillCrossAxis();
    } else {
      return UIComponent::wantsToFillCrossAxis();
    }
  }

 protected:
  bool processChildTaps(const UITapEvent &event) override {
    if (params_.child) {
      return params_.child->processTap(event);
    }
    return false;
  }

  ViewParams params_;
};
