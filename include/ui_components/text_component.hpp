#pragma once

#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>

#include "color.hpp"
#include "ui_component.hpp"

/**
 * @brief The FontWeight enum represents the font weight of a Text element.
 *
 * e.g. FontWeight::Normal, FontWeight::Bold, FontWeight::Light
 */
enum class FontWeight { Normal, Bold, Light };

/**
 * @brief A basic text rendering UI component.
 *
 * The Text class represents a drawable text element that can be customized
 * with a string, font size, and color. It inherits from UIComponent and
 * overrides layout and draw methods.
 *
 * Supports flexible construction:
 * - Text("Hello World")                       // default color & font size
 * - Text("Hello World", 18.0f)                // custom font size
 * - Text("Hello World", Color::Red())         // custom color
 * - Text("Hello World", Color::Blue(), 20.0f) // custom color & font size
 */
class TextComponent : public UIComponent {
 public:
  TextComponent(const std::string& text, Color color = Color::Black(), float fontSize = 16.0f,
                FontWeight weight = FontWeight::Normal, bool underline = false, bool italic = false);

  TextComponent(const std::string& text, float fontSize);
  TextComponent(const std::string& text, Color color);
  TextComponent(const std::string& text, float fontSize, Color color);
  TextComponent(const std::string& text, float fontSize, FontWeight weight);
  TextComponent(const std::string& text, float fontSize, FontWeight weight, bool underline = false,
                bool italic = false);

  void layout(float parentWidth, float parentHeight) override;

  void draw(SkCanvas* canvas) override;

 private:
  std::string text_;
  Color color_;
  float fontSize_;
  FontWeight weight_;
  bool underline_;
  bool italic_;

  SkPaint paint_;
  SkFont font_;
};
