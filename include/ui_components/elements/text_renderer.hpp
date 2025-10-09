#pragma once

#include <string>

#include "basic/ui_component.hpp"
#include "core/text_style.hpp"
#include "elements/paragraph_builder.hpp"

class TextRenderer : public UIComponent {
 public:
  TextRenderer(const std::string &text, const TextStyle &style);

  TextRenderer(const TextRenderer &) = delete;
  TextRenderer &operator=(const TextRenderer &) = delete;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;
  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const override;

 private:
  std::string text_;
  ParagraphBuilder paragraphBuilder_;
};
