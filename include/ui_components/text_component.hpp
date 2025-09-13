#pragma once

#include <fmt/format.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <sstream>
#include <vector>
#include <hb.h>

#include "./color.hpp"
#include "./size.hpp"
#include "ui_component.hpp"

struct TextMetrics {
  float ascent = 0.0f;         // Distance from baseline to top of glyphs (positive value)
  float descent = 0.0f;        // Distance from baseline to bottom of glyphs (positive value)
  float leading = 0.0f;        // Recommended line spacing
  float x_advance = 0.0f;      // Total width of the text string
  float x_max_advance = 0.0f;  // Use to track the widest line for multi-line
};

/**
 * @brief The FontWeight enum represents the font weight of a Text element.
 *
 * e.g. FontWeight::Normal, FontWeight::Bold, FontWeight::Light
 */
enum class FontWeight { Normal, Bold, Light };

inline std::ostream& operator<<(std::ostream& os, const FontWeight& weight) {
  switch (weight) {
    case FontWeight::Normal:
      os << "Normal";
      break;
    case FontWeight::Bold:
      os << "Bold";
      break;
    case FontWeight::Light:
      os << "Light";
      break;
  }
  return os;
}

enum class TextDecoration { underline, strikethrough, none };

inline std::ostream& operator<<(std::ostream& os, const TextDecoration& decoration) {
  switch (decoration) {
    case TextDecoration::underline:
      os << "underline";
      break;
    case TextDecoration::strikethrough:
      os << "strikethrough";
      break;
    case TextDecoration::none:
      os << "none";
      break;
  }
  return os;
}

struct TextStyle {
  Color color = Color::Black();
  float fontSize = 16.0f;
  FontWeight weight = FontWeight::Normal;
  TextDecoration decoration = TextDecoration::none;
  bool italic = false;

  std::string toString() const {
    std::ostringstream os;
    os << "TextStyle { ";
    os << "color: " << color.toString() << ", ";
    os << "fontSize: " << fontSize << ", ";
    os << "weight: " << weight << ", ";
    os << "decoration: " << decoration << ", ";
    os << "italic: " << (italic ? "true" : "false");
    os << "}";
    return os.str();
  }
};

/**
 * @brief A basic text rendering UI component.
 *
 * The Text class represents a drawable text element that can be customized
 * with a string, font size, and color. It inherits from UIComponent and
 * overrides layout and draw methods.
 *
 */
class TextRenderer : public UIComponent {
 public:
  TextRenderer(const std::string& text, const TextStyle& style = TextStyle());

  void layout(UISize size) override;

  void draw(SkCanvas* canvas) override;

 private:
  std::string text_;
  TextStyle style_;

  SkPaint paint_;
  SkFont font_;
  TextMetrics text_metrics_;
  float text_bounds_offset_x_ = 0.0f;
  float text_bounds_offset_y_ = 0.0f;

  SkRect text_bounds_;
  std::vector<std::string> line_;

 protected:
  void debugFillProperties(std::ostringstream& os, int indent) const override;

  std::vector<std::string> splitByNewlines(const std::string& text);

  std::string breakLongWord(const SkFont& font, const std::string& word, float maxWidth);

  void breakTextIntoLines(const SkFont& font, float maxWidth);
};
