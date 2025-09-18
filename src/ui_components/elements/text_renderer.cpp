#include <fmt/base.h>
#include <fmt/format.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontTypes.h>
#include <include/core/SkRect.h>
#include <include/core/SkTextBlob.h>
#include <sstream>
#include <string>
#include <vector>
#include <unicode/uchar.h>
#include <unicode/utf8.h>

#include "size.hpp"
#include "elements/text_renderer.hpp"

inline bool isUnicodeWhitespace(UChar32 c) {
  return c == 0x0020 ||  // Space
         c == 0x0009 ||  // Tab
         c == 0x000A ||  // Line feed
         c == 0x000D ||  // Carriage return
         c == 0x00A0 ||  // Non-breaking space
         c == 0x1680 ||  // Ogham space mark
         c == 0x2000 ||  // En quad
         c == 0x2001 ||  // Em quad
         c == 0x2002 ||  // En space
         c == 0x2003 ||  // Em space
         c == 0x2004 ||  // Three-per-em space
         c == 0x2005 ||  // Four-per-em space
         c == 0x2006 ||  // Six-per-em space
         c == 0x2007 ||  // Figure space
         c == 0x2008 ||  // Punctuation space
         c == 0x2009 ||  // Thin space
         c == 0x200A ||  // Hair space
         c == 0x202F ||  // Narrow no-break space
         c == 0x205F ||  // Medium mathematical space
         c == 0x3000;    // Ideographic space
}

TextRenderer::TextRenderer(const std::string &text, const TextStyle &style) : text_(text), style_(style) {
  paint_.setColor(style_.color);
  paint_.setAntiAlias(true);
}

UISize TextRenderer::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize intrinsicSize = {0, 0};
  SkFont font = configureFont();
  font_ = font;

  breakTextIntoLinesConst(font, constraints.minWidth);

  SkFontMetrics fontMetrics;
  font.getMetrics(&fontMetrics);

  float totalLineHeight = -fontMetrics.fAscent + fontMetrics.fDescent + fontMetrics.fLeading;
  intrinsicSize.width = text_metrics_.x_max_advance;
  intrinsicSize.height = totalLineHeight * static_cast<float>(line_.size());

  return intrinsicSize;
}

void TextRenderer::layout(UISize size) {
  SkFont font = configureFont();
  font_ = font;

  breakTextIntoLines(font, size.width);

  SkFontMetrics fontMetrics;
  font.getMetrics(&fontMetrics);

  float capHeight = fontMetrics.fCapHeight > 0 ? fontMetrics.fCapHeight : (-fontMetrics.fAscent * 0.7f);
  float xHeight = fontMetrics.fXHeight > 0 ? fontMetrics.fXHeight : (-fontMetrics.fAscent * 0.5f);

  float totalLineHeight = -fontMetrics.fAscent + fontMetrics.fDescent + fontMetrics.fLeading;
  bounds_.width = text_metrics_.x_max_advance;
  bounds_.height = totalLineHeight * static_cast<float>(line_.size());

  text_metrics_.ascent = -fontMetrics.fAscent;
  text_metrics_.descent = fontMetrics.fDescent;
  text_metrics_.leading = fontMetrics.fLeading;
  text_metrics_.visual_center_offset = (capHeight + xHeight) / 2.0f;

  SkRect singleTextBounds;
  font.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &singleTextBounds);
  text_bounds_ = singleTextBounds;
  text_bounds_offset_x_ = singleTextBounds.fLeft;
  text_bounds_offset_y_ = -fontMetrics.fTop;
}

void TextRenderer::draw(SkCanvas *canvas) {
  if (line_.empty()) return;

  float lineHeight = text_metrics_.ascent + text_metrics_.descent + text_metrics_.leading;
  float totalTextHeight = lineHeight * line_.size();

  float startY = bounds_.y + (bounds_.height - totalTextHeight) / 2.0f + text_metrics_.ascent;
  float drawX = bounds_.x - text_bounds_offset_x_;
  float currentY = startY;

  for (const auto &line : line_) {
    canvas->drawSimpleText(line.c_str(), line.length(), SkTextEncoding::kUTF8, drawX, currentY, font_, paint_);
    currentY += lineHeight;
  }

  UIComponent::draw(canvas);
}

inline std::vector<std::string> TextRenderer::splitByNewlines(const std::string &text) {
  std::vector<std::string> paragraphs;
  std::string currentParagraph;

  for (size_t i = 0; i < text.length(); ++i) {
    if (text[i] == '\n') {
      paragraphs.push_back(currentParagraph);
      currentParagraph.clear();
    } else if (text[i] == '\r') {
      // Handle Windows-style line endings (\r\n) or Mac-style (\r)
      paragraphs.push_back(currentParagraph);
      currentParagraph.clear();
      // Skip the \n if it follows \r
      if (i + 1 < text.length() && text[i + 1] == '\n') {
        ++i;
      }
    } else {
      currentParagraph += text[i];
    }
  }

  paragraphs.push_back(currentParagraph);
  return paragraphs;
}

inline std::vector<std::string> splitIntoWords(const std::string &paragraph) {
  std::vector<std::string> words;
  std::string currentWord;
  UChar32 c;
  int32_t i = 0;

  while (i < paragraph.length()) {
    U8_NEXT(paragraph.c_str(), i, paragraph.length(), c);
    if (c < 0) {
      fmt::print("Invalid UTF-8: {}", c);
      break;  // Invalid UTF-8
    }
    if (isUnicodeWhitespace(c)) {
      if (!currentWord.empty()) {
        words.push_back(currentWord);
        currentWord.clear();
      }
    } else {
      currentWord.append(paragraph, i - U8_LENGTH(c), U8_LENGTH(c));
    }
  }
  if (!currentWord.empty()) {
    words.push_back(currentWord);
  }
  return words;
}

void TextRenderer::breakTextIntoLinesConst(const SkFont &font, float maxWidth) noexcept {
  line_.clear();
  text_metrics_.x_max_advance = 0.0f;

  // First, split text by explicit line breaks (\n)
  std::vector<std::string> paragraphs = splitByNewlines(text_);

  for (const auto &paragraph : paragraphs) {
    if (paragraph.empty()) {
      // Handle empty lines (consecutive \n characters)
      line_.push_back("");
      continue;
    }

    // Process each paragraph for word wrapping
    std::string currentLine;
    std::istringstream iss(paragraph);
    std::string word;

    while (iss >> word) {
      std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
      SkRect testBounds;
      font.measureText(testLine.c_str(), testLine.length(), SkTextEncoding::kUTF8, &testBounds);

      if (testBounds.width() <= maxWidth || currentLine.empty()) {
        currentLine = testLine;
      } else {
        if (!currentLine.empty()) {
          line_.push_back(currentLine);
          SkRect currentLineBounds;
          font.measureText(currentLine.c_str(), currentLine.length(), SkTextEncoding::kUTF8, &currentLineBounds);
          text_metrics_.x_max_advance = std::max(text_metrics_.x_max_advance, currentLineBounds.width());
        }

        currentLine = word;

        SkRect wordBounds;
        font.measureText(word.c_str(), word.length(), SkTextEncoding::kUTF8, &wordBounds);
        if (wordBounds.width() > maxWidth) {
          currentLine = breakLongWord(font, word, maxWidth);
        }
      }
    }

    // Add the last line of this paragraph if it exists
    if (!currentLine.empty()) {
      line_.push_back(currentLine);
      SkRect lastLineBounds;
      font.measureText(currentLine.c_str(), currentLine.length(), SkTextEncoding::kUTF8, &lastLineBounds);
      text_metrics_.x_max_advance = std::max(text_metrics_.x_max_advance, lastLineBounds.width());
    }
  }
}

void TextRenderer::breakTextIntoLines(const SkFont &font, float maxWidth) noexcept {
  breakTextIntoLinesConst(font, maxWidth);
}

void TextRenderer::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "text: " << fmt::format("\"{}\"", text_) << "\n";
  os << pad << "style: " << style_.toString() << "\n";
  os << pad << "max_advance: " << text_metrics_.x_max_advance << "\n";
  os << pad << "line_count: " << line_.size() << "\n";
}
