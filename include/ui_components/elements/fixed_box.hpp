#pragma once

#include <memory>
#include <sstream>
#include <utility>

#include "basic/ui_component.hpp"
#include "size.hpp"
#include "tap_event.hpp"
#include "ui_alignment.hpp"

struct FixedBoxParam {
  UISizing size = UISizing::Fixed(0, 0);
  std::shared_ptr<UIComponent> child = nullptr;
  UIAlignment alignment = UIAlignment::Center;
};

class FixedBox : public UIComponent {
 public:
  FixedBox(FixedBoxParam param = {}) : params_(std::move(param)) {};

  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const override;

  bool processChildTaps(const UITapEvent &event) override {
    if (params_.child) {
      UITapEvent localEvent = event;
      localEvent.x -= bounds_.x;
      localEvent.y -= bounds_.y;

      return params_.child->processTap(localEvent);
    }
    return false;
  }

  bool wantsToFillMainAxis() const override { return params_.size.isGrowHeight(); }
  bool wantsToFillCrossAxis() const override { return params_.size.isGrowWidth(); }

  float computeWidth() const noexcept;
  float computeHeight() const noexcept;

 private:
  FixedBoxParam params_;
  mutable std::vector<std::shared_ptr<UIComponent>> cached_children_;
};

inline void FixedBox::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "size: " << params_.size << "\n";
  os << pad << "hasChild: " << (params_.child ? "true" : "false") << "\n";
  os << pad << "alignment: " << params_.alignment << "\n";
}
