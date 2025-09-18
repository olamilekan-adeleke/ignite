#pragma once

#include "color.hpp"
#include "ui_alignment.hpp"
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
  MainAxisSize mainAxisSize = MainAxisSize::FIT;
  std::shared_ptr<UIComponent> child = nullptr;
};

class View : public UIComponent {
 public:
  View(const ViewParams &params = ViewParams());

  View(const View &) = delete;
  View &operator=(const View &) = delete;

  void layout(UISize size) override;
  void draw(SkCanvas *canvas) override;

  bool wantsToFillMainAxis() const override {
    if (params_.child && params_.mainAxisSize == MainAxisSize::FIT) {
      return params_.child->wantsToFillMainAxis();
    } else {
      return params_.mainAxisSize == MainAxisSize::FILL;
    }
  }

  bool wantsToFillCrossAxis() const override {
    if (params_.child) {
      return params_.child->wantsToFillCrossAxis();
    } else {
      return UIComponent::wantsToFillCrossAxis();
    }
  }

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  bool processChildTaps(const UITapEvent &event) override {
    if (params_.child) {
      return params_.child->processTap(event);
    }
    return false;
  }

  ViewParams params_;
};
