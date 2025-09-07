#pragma once

#include "ui_component.hpp"

struct OpacityParams {
  float opacity = 1.0f;
  std::shared_ptr<UIComponent> child = nullptr;
};

class Opacity : public UIComponent {
 public:
  Opacity(const OpacityParams& param = {});

  Opacity(const Opacity&) = delete;
  Opacity& operator=(const Opacity&) = delete;

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

 protected:
  OpacityParams param_;
};
