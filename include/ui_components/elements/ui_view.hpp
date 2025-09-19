#pragma once

#include <sstream>
#include "basic/ui_component.hpp"
#include "color.hpp"
#include "size.hpp"
#include "ui_alignment.hpp"
#include "ui_edge_insets.hpp"

struct ViewParams {
  UIEdgeInsets insets = {0.0f, 0.0f, 0.0f, 0.0f};
  UIEdgeInsets margin = {0.0f, 0.0f, 0.0f, 0.0f};
  Color backgroundColor = Color::Clear();
  float borderRadius = 0.0f;
  bool antiAlias = true;
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

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  bool wantsToFillMainAxis() const override {
    if (params_.child && params_.mainAxisSize == MainAxisSize::FIT) {
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

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  bool processChildTaps(const UITapEvent &event) override {
    if (params_.child) {
      return params_.child->processTap(event);
    }
    return false;
  }

  void debugFillProperties(std::ostringstream &os, int indent) const override;

  ViewParams params_;
};

inline void View::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "insets: " << params_.insets.toString() << "\n";
  os << pad << "margin: " << params_.margin.toString() << "\n";
  os << pad << "backgroundColor: " << params_.backgroundColor.toString() << "\n";
  os << pad << "borderRadius: " << params_.borderRadius << "\n";
  os << pad << "antiAlias: " << (params_.antiAlias ? "true" : "false") << "\n";
  os << pad << "tappable: " << (params_.tappable ? "true" : "false") << "\n";
}
