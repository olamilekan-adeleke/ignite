#pragma once

#include <fmt/base.h>

#include <string>

#include "basic/ui_component.hpp"
#include "component/component.hpp"
#include "elements/paragraph_builder.hpp"
#include "foundation/foundation.hpp"
#include "foundation/utils/key.hpp"

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

class RenderText : public RenderObject {
 public:
  RenderText(const std::string &text, const TextStyle &style) : paragraphBuilder_(ParagraphBuilder(text, style)) {}

  void performLayout(UIConstraints constraints) noexcept override {
    float width = constraints.maxWidth;
    paragraphBuilder_.layout(width);

    float actualWidth = paragraphBuilder_.getWidth();
    float actualHeight = paragraphBuilder_.getHeight();

    UISize size{
        .width = std::clamp(actualWidth, constraints.minWidth, constraints.maxWidth),
        .height = actualHeight,
    };
    setSize(size);
  }

  void paint(SkCanvas *canvas) noexcept override {
    const auto &bounds = getBounds();
    paragraphBuilder_.draw(canvas, SkPoint::Make(bounds.x, bounds.y));

    RenderObject::paint(canvas);
  }

  void setText(const std::string &text) { paragraphBuilder_.setText(text); }

  void setStyle(const TextStyle &style) { paragraphBuilder_.setStyle(style); }

 private:
  ParagraphBuilder paragraphBuilder_;
};

class Text : public Component {
 public:
  Text(const std::string &text, const TextStyle &style = {}, const UIKey &key = AUTO_KEY)
      : text_(text), style_(style), Component(key) {};

  UIElementPtr createElement() override;

  RenderObjectPtr createRenderObject() const noexcept override;

  void updateRenderObject(RenderObjectPtr ro) noexcept override;

 private:
  std::string text_;
  TextStyle style_;
};
