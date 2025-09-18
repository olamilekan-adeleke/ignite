#pragma once

#include <include/core/SkImage.h>
#include <string>
#include "ui_component.hpp"

struct ImageParams {
  std::string path;
  float width = 0.0f;
  float height = 0.0f;
  float opacity = 1.0f;
};

class UIImage : public UIComponent {
 public:
  UIImage(const ImageParams& params = {});

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

 protected:
  ImageParams params_;
  sk_sp<SkImage> imageData_;

  void debugFillProperties(std::ostringstream& os, int indent) const override;
};
