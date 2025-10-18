#pragma once

#include <string>

#include "basic/ui_component.hpp"
#include "elements/paragraph_builder.hpp"
#include "foundation/foundation.hpp"

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
  TextStyle style_;
  ParagraphBuilder paragraphBuilder_;

  uint64_t layoutHashCode() const noexcept {
    const auto textKey = fnv1a(text_);
    const auto key = fmt::format("{}-{}-{}-{}", textKey, style_.fontSize, style_.weight.toString(), style_.maxLines);
    return fnv1a(key);
  }

  uint64_t drawHashCode() const noexcept {
    const auto textKey = fnv1a(text_);
    auto const key = fmt::format("{}-{}-{}-{}-{}-{}",
                                 textKey,
                                 style_.fontSize,
                                 style_.weight.toString(),
                                 style_.maxLines,
                                 style_.textAlignment.toString(),
                                 style_.color.toString());
    return fnv1a(key);
  }
};
