#pragma once

#include <include/core/SkCanvas.h>

#include <sstream>

#include "basic/ui_component.hpp"
#include "color.hpp"
#include "hash_helper.hpp"
#include "size.hpp"

struct CheckBoxParams {
  bool checked = false;
  UISize size{18, 18};
  float radius = 4.0f;
  Color fillColor = Color::Blue();
  Color inactiveBorderColor = Color::Gray();
  Color checkColor = Color::White();
  TapListener onTap = nullptr;

  uint64_t layoutHashCode() const noexcept {
    const auto key = fmt::format("{}", size.toString());
    return fnv1a(key);
  }

  uint64_t drawHashCode() const noexcept {
    const auto key = fmt::format(
        "{}-{}-{}-{}-{}", checked, fillColor.toString(), checkColor.toString(), inactiveBorderColor.toString(), radius);
    return fnv1a(key);
  }
};

class CheckBoxRender : public UIComponent {
 public:
  CheckBoxRender(const CheckBoxParams &params) : params_(params) {
    if (params_.onTap != nullptr) setTapListener(params_.onTap);
  }

  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  void markHasDirty(const UIMarkDirtyType &type, const UIMarkDirtyCaller &caller) noexcept override;

 protected:
  bool processChildTaps(const UITapEvent &event) override;

  void debugFillProperties(std::ostringstream &os, int indent) const override;

 private:
  CheckBoxParams params_;

  void drawCheckBox(SkCanvas *canvas) const;
};

inline bool CheckBoxRender::processChildTaps(const UITapEvent &event) {
  if (params_.onTap == nullptr) return false;

  UITapEvent localEvent = event;
  localEvent.x -= bounds_.x;
  localEvent.y -= bounds_.y;
  return onTap(localEvent, bounds_);
}

inline void CheckBoxRender::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "enable: " << (params_.checked ? "true" : "false") << "\n";
  os << pad << "fillColor: " << params_.fillColor.toString() << "\n";
  os << pad << "checkColor: " << params_.checkColor.toString() << "\n";
  os << pad << "onTap: " << (params_.onTap ? "true" : "false") << "\n";
}
