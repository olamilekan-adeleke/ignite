#pragma once

#include <include/core/SkPaint.h>

#include "color.hpp"
#include "ui_component.hpp"

class Text : public UIComponent {
 public:
  Text(const std::string& text, Color color = Color::Black(), float fontSize = 16.0f);

  void layout(float parentWidth, float parentHeight) override;

  void draw(SkCanvas* canvas) override;

 private:
  std::string text_;
  Color color_;
  float fontSize_;
  SkPaint paint_;
};
