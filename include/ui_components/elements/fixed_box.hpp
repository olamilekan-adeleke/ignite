#pragma once

#include <memory>
#include <sstream>
#include <utility>

#include "basic/ui_component.hpp"
#include "ui_alignment.hpp"

struct FixedBoxParam {
  UISize size = {-1.f, -1.f};
  std::shared_ptr<UIComponent> child = nullptr;
  UIAlignment alignment = UIAlignment::Center;
};

class FixedBox : public UIComponent {
 public:
  FixedBox(FixedBoxParam param = {}) : params_(std::move(param)) {};

  void layout(UISize size) override;
  void draw(SkCanvas *canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const override;

  bool processChildTaps(const UITapEvent &event) override {
    if (params_.child) return params_.child->processTap(event);
    return false;
  }

 private:
  FixedBoxParam params_;
  mutable std::vector<std::shared_ptr<UIComponent>> cached_children_;
};

inline void FixedBox::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "size: " << params_.size << "\n";
  os << pad << "hasChild: " << (params_.child ? "true" : "false") << "\n";
}
