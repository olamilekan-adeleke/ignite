#pragma once

#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>

#include "color.hpp"
#include "ui_component.hpp"

struct TextMetrics {
  float ascent = 0.0f;     // Distance from baseline to top of glyphs (positive value)
  float descent = 0.0f;    // Distance from baseline to bottom of glyphs (positive value)
  float leading = 0.0f;    // Recommended line spacing
  float x_advance = 0.0f;  // Total width of the text string
};

/**
 * @brief The FontWeight enum represents the font weight of a Text element.
 *
 * e.g. FontWeight::Normal, FontWeight::Bold, FontWeight::Light
 */
enum class FontWeight { Normal, Bold, Light };

enum class TextDecoration { underline, strickThrought, none };

struct TextStyle {
  Color color = Color::Black();
  float fontSize = 16.0f;
  FontWeight weight = FontWeight::Normal;
  TextDecoration decoration = TextDecoration::none;
  bool italic = false;
};

/**
 * @brief A basic text rendering UI component.
 *
 * The Text class represents a drawable text element that can be customized
 * with a string, font size, and color. It inherits from UIComponent and
 * overrides layout and draw methods.
 *
 */
class TextComponent : public UIComponent {
 public:
  TextComponent(const std::string& text, const TextStyle& style = TextStyle());

  void layout(float parentWidth, float parentHeight) override;

  void draw(SkCanvas* canvas) override;

 private:
  std::string text_;
  TextStyle style_;

  SkPaint paint_;
  SkFont font_;
  TextMetrics text_metrics_;
  float text_bounds_offset_x_;
  float text_bounds_offset_y_;
};
