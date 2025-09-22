#pragma once

#include <fmt/format.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <unicode/umachine.h>
#include <unicode/utf8.h>
#include <sstream>
#include <vector>

#include "./color.hpp"
#include "./size.hpp"
#include "basic/ui_component.hpp"
#include "rect.hpp"
#include "ui_manager.hpp"

struct TextMetrics {
  float ascent = 0.0f;                // Distance from baseline to top of glyphs (positive value)
  float descent = 0.0f;               // Distance from baseline to bottom of glyphs (positive value)
  float leading = 0.0f;               // Recommended line spacing
  float x_advance = 0.0f;             // Total width of the text string
  float x_max_advance = 0.0f;         // Use to track the widest line for multi-line
  float visual_center_offset = 0.0f;  // Use to center the text vertically
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

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

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
  SkFont configureFont();

  void debugFillProperties(std::ostringstream& os, int indent) const override;

  std::vector<std::string> splitByNewlines(const std::string& text);
  std::string breakLongWord(const SkFont& font, const std::string& word, float maxWidth) noexcept;
  void breakTextIntoLines(const SkFont& font, float maxWidth) noexcept;
  void breakTextIntoLinesConst(const SkFont& font, float maxWidth) noexcept;
};

inline SkFont TextRenderer::configureFont() {
  int skWeight = SkFontStyle::kNormal_Weight;
  if (style_.weight == FontWeight::Bold) {
    skWeight = SkFontStyle::kBold_Weight;
  } else if (style_.weight == FontWeight::Light) {
    skWeight = SkFontStyle::kLight_Weight;
  }

  int slant = style_.italic ? SkFontStyle::kItalic_Slant : SkFontStyle::kUpright_Slant;
  SkFontStyle skStyle(skWeight, SkFontStyle::kNormal_Width, static_cast<SkFontStyle::Slant>(slant));

  SkFont font = UIManager::instance().defaultFont();
  sk_sp<SkTypeface> typeface = UIManager::instance().fontManager()->matchFamilyStyle(nullptr, skStyle);
  font.setTypeface(typeface ? std::move(typeface) : SkTypeface::MakeEmpty());
  font.setSize(style_.fontSize);
  return font;
}

inline std::string TextRenderer::breakLongWord(const SkFont& font, const std::string& word, float maxWidth) noexcept {
  std::string currentChunk;
  std::string result;
  UChar32 c;
  int32_t i = 0;

  while (i < word.length()) {
    U8_NEXT(word.c_str(), i, word.length(), c);
    if (c < 0) break;  // Invalid UTF-8
    std::string testChunk = currentChunk;
    testChunk.append(word, i - U8_LENGTH(c), U8_LENGTH(c));
    SkRect bounds;
    font.measureText(testChunk.c_str(), testChunk.length(), SkTextEncoding::kUTF8, &bounds);

    if (bounds.width() <= maxWidth) {
      currentChunk = testChunk;
    } else {
      if (!currentChunk.empty()) {
        line_.push_back(currentChunk);
        SkRect chunkBounds;
        font.measureText(currentChunk.c_str(), currentChunk.length(), SkTextEncoding::kUTF8, &chunkBounds);
        text_metrics_.x_max_advance = std::max(text_metrics_.x_max_advance, chunkBounds.width());
      }
      currentChunk.clear();
      currentChunk.append(word, i - U8_LENGTH(c), U8_LENGTH(c));
    }
  }

  return currentChunk;
}

inline void TextRenderer::debugFillProperties(std::ostringstream& os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "text: " << fmt::format("\"{}\"", text_) << "\n";
  os << pad << "style: " << style_.toString() << "\n";
  os << pad << "max_advance: " << text_metrics_.x_max_advance << "\n";
  os << pad << "line_count: " << line_.size() << "\n";
}
