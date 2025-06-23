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

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas* canvas) override;

 protected:
  ImageParams params_;
  sk_sp<SkImage> imageData_;
};
