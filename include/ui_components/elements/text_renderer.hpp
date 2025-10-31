#pragma once

#include <fmt/base.h>

#include <string>

#include "component/component.hpp"
#include "elements/paragraph_builder.hpp"
#include "foundation/foundation.hpp"
#include "foundation/utils/key.hpp"

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
  Text(const std::string &text, const TextStyle &style = {}, const UIKey &key = {})
      : text_(text), style_(style), Component(key) {};

  UIElementPtr createElement() override;

  RenderObjectPtr createRenderObject() const noexcept override;

  void updateRenderObject(RenderObjectPtr ro) noexcept override;

 private:
  std::string text_;
  TextStyle style_;
};
