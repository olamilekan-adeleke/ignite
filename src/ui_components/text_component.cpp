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

#include "text_component.hpp"
#include "ui_manager.hpp"

TextRenderer::TextRenderer(const std::string &text, const TextStyle &style) : text_(text), style_(style) {
  paint_.setColor(style_.color);
  paint_.setAntiAlias(true);
}

void TextRenderer::layout(UISize size) {
  int skWeight = SkFontStyle::kNormal_Weight;
  if (style_.weight == FontWeight::Bold) {
    skWeight = SkFontStyle::kBold_Weight;
  } else if (style_.weight == FontWeight::Light) {
    skWeight = SkFontStyle::kLight_Weight;
  }

  int slant = style_.italic ? SkFontStyle::kItalic_Slant : SkFontStyle::kUpright_Slant;
  SkFontStyle style(skWeight, SkFontStyle::kNormal_Width, (SkFontStyle::Slant)slant);

  SkFont font = UIManager::instance().defaultFont();
  sk_sp<SkTypeface> typeface = UIManager::instance().fontManager()->matchFamilyStyle(nullptr, style);
  font.setTypeface(typeface ? std::move(typeface) : SkTypeface::MakeEmpty());
  font.setSize(style_.fontSize);

  font_ = font;

  breakTextIntoLines(font, size.width);

  SkFontMetrics fontMetrics;
  font_.getMetrics(&fontMetrics);

  float totalLineHeight = -fontMetrics.fAscent + fontMetrics.fDescent + fontMetrics.fLeading;
  bounds_.width = text_metrics_.x_max_advance;
  bounds_.height = totalLineHeight * static_cast<float>(line_.size());

  text_metrics_.ascent = -fontMetrics.fAscent;
  text_metrics_.descent = fontMetrics.fDescent;
  text_metrics_.leading = fontMetrics.fLeading;

  SkRect singleTextBounds;
  font.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &singleTextBounds);
  text_bounds_ = singleTextBounds;
  text_bounds_offset_x_ = singleTextBounds.fLeft;
  text_bounds_offset_y_ = -fontMetrics.fTop;
  // text_bounds_offset_y_ = -fontMetrics.fAscent;
}

void TextRenderer::draw(SkCanvas *canvas) {
  if (line_.empty()) return;

  float baselineY = bounds_.y + text_metrics_.ascent;
  float lineHeight = text_metrics_.ascent + text_metrics_.descent + text_metrics_.leading;
  float drawX = bounds_.x - text_bounds_offset_x_;

  for (const auto &line : line_) {
    canvas->drawSimpleText(line.c_str(), line.length(), SkTextEncoding::kUTF8, drawX, baselineY, font_, paint_);
    baselineY += lineHeight;
  }

  UIComponent::draw(canvas);
}

void TextRenderer::breakTextIntoLines(const SkFont &font, float maxWidth) {
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

  // Ensure we have a minimum width
  if (text_metrics_.x_max_advance < 1.0f) {
    text_metrics_.x_max_advance = 1.0f;
  }
}

std::vector<std::string> TextRenderer::splitByNewlines(const std::string &text) {
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

std::string TextRenderer::breakLongWord(const SkFont &font, const std::string &word, float maxWidth) {
  std::string result;
  std::string currentChunk;

  // UTF-8 safe character iteration would be better, but this is a simple approach
  for (size_t i = 0; i < word.length(); ++i) {
    std::string testChunk = currentChunk + word[i];
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
      currentChunk = word[i];
    }
  }

  return currentChunk;
}

void TextRenderer::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "text: " << fmt::format("\"{}\"", text_) << "\n";
  os << pad << "style: " << style_.toString() << "\n";
  os << pad << "max_advance: " << text_metrics_.x_max_advance << "\n";
  os << pad << "line_count: " << line_.size() << "\n";
}
