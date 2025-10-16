#pragma once

#include <sstream>

#include "basic/ui_component.hpp"
#include "color.hpp"
#include "ui_edge_insets.hpp"

struct ViewParams {
  UIEdgeInsets insets = {0.0f, 0.0f, 0.0f, 0.0f};
  UIEdgeInsets margin = {0.0f, 0.0f, 0.0f, 0.0f};
  Color backgroundColor = Color::Clear();
  float borderRadius = 0.0f;
  bool antiAlias = true;
  bool tappable = true;
  TapListener onTap = nullptr;
  std::shared_ptr<UIComponent> child = nullptr;
};

class View : public UIComponent {
 public:
  View(const ViewParams &params = ViewParams()) : params_(params) {
    setTapListener(params_.onTap);
    if (params_.child) setChild(params_.child);
  }

  View(const View &) = delete;
  View &operator=(const View &) = delete;

  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

 protected:
  bool processChildTaps(const UITapEvent &event) override {
    if (params_.onTap) {
      return UIComponent::onTap(event, bounds_);
    }

    if (params_.child) {
      UITapEvent localEvent = event;
      localEvent.x -= bounds_.x;
      localEvent.y -= bounds_.y;
      return params_.child->processTap(localEvent);
    }

    return false;
  }

  void debugFillProperties(std::ostringstream &os, int indent) const override;

 private:
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
