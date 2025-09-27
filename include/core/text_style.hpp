#pragma once

#include <sstream>
#include "color.hpp"
#include "modules/skparagraph/include/Paragraph.h"

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
  skia::textlayout::TextAlign textAlign = skia::textlayout::TextAlign::kLeft;
  int maxLines = 0;

  std::string toString() const {
    std::ostringstream os;
    os << "TextStyle { ";
    os << "color: " << color.toString() << ", ";
    os << "fontSize: " << fontSize << ", ";
    os << "weight: " << weight << ", ";
    os << "decoration: " << decoration << ", ";
    os << "italic: " << (italic ? "true" : "false") << ", ";
    os << "textAlign: " << static_cast<int>(textAlign) << ", ";
    os << "maxLines: " << maxLines;
    os << "}";
    return os.str();
  }
};
