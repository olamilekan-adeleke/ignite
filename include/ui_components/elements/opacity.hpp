#pragma once

#include <sstream>
#include "size.hpp"
#include "basic/ui_component.hpp"

struct OpacityParams {
  float opacity = 1.0f;
  std::shared_ptr<UIComponent> child = nullptr;
};

class Opacity : public UIComponent {
 public:
  Opacity(const OpacityParams& param = {});

  Opacity(const Opacity&) = delete;
  Opacity& operator=(const Opacity&) = delete;

  void layout(UIConstraints size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

 protected:
  OpacityParams param_;

  void debugFillProperties(std::ostringstream& os, int indent) const override;
};

inline void Opacity::debugFillProperties(std::ostringstream& os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "opacity: " << param_.opacity << "\n";
}
