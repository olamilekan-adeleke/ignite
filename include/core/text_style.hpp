#pragma once

#include <include/core/SkFontStyle.h>
#include <modules/skparagraph/include/DartTypes.h>
#include <modules/skparagraph/include/TextStyle.h>
#include <iostream>
#include <sstream>
#include "color.hpp"

/**
 * @brief The FontWeight enum represents the font weight of a Text element.
 *
 * e.g. FontWeight::Normal, FontWeight::Bold, FontWeight::Light
 */
struct FontWeight {
  enum Value { Normal, Bold, Light } value;

  FontWeight(Value v) : value(v) {}

  operator SkFontStyle() const {
    switch (value) {
      case Normal:
        return SkFontStyle(SkFontStyle::kNormal_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);
      case Bold:
        return SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);
      case Light:
        return SkFontStyle(SkFontStyle::kLight_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);
    }
  }
};

inline std::ostream& operator<<(std::ostream& os, const FontWeight& weight) {
  switch (weight.value) {
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

struct TextAlignment {
  enum Value { left, center, right, justified } value;

  TextAlignment(Value v) : value(v) {}

  operator skia::textlayout::TextAlign() const {
    switch (value) {
      case left:
        return skia::textlayout::TextAlign::kLeft;
      case center:
        return skia::textlayout::TextAlign::kCenter;
      case right:
        return skia::textlayout::TextAlign::kRight;
      case justified:
        return skia::textlayout::TextAlign::kJustify;
    }
  }
};

inline std::ostream& operator<<(std::ostream& os, const TextAlignment& alignment) {
  switch (alignment.value) {
    case TextAlignment::left:
      os << "left";
      break;
    case TextAlignment::center:
      os << "center";
      break;
    case TextAlignment::right:
      os << "right";
      break;
    case TextAlignment::justified:
      os << "justified";
      break;
  }
  return os;
}

struct TextDecoration {
  enum Value { underline, strikethrough, none } value;

  TextDecoration(Value v) : value(v) {}

  operator skia::textlayout::TextDecoration() const {
    switch (value) {
      case underline:
        return skia::textlayout::kUnderline;
      case strikethrough:
        return skia::textlayout::kLineThrough;
      case none:
        return skia::textlayout::kNoDecoration;
    }
  }
};

inline std::ostream& operator<<(std::ostream& os, const TextDecoration& decoration) {
  switch (decoration.value) {
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
  TextAlignment textAlignment = TextAlignment::left;
  int maxLines = 0;

  std::string toString() const {
    std::ostringstream os;
    os << "TextStyle { ";
    os << "color: " << color.toString() << ", ";
    os << "fontSize: " << fontSize << ", ";
    os << "weight: " << weight << ", ";
    os << "decoration: " << decoration << ", ";
    os << "italic: " << (italic ? "true" : "false") << ", ";
    os << "textAlign: " << textAlignment << ", ";
    os << "maxLines: " << maxLines;
    os << "}";
    return os.str();
  }
};
