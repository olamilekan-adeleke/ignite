#include <fmt/base.h>
#include <fmt/format.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontTypes.h>
#include <include/core/SkRect.h>
#include <include/core/SkTextBlob.h>
#include <sstream>

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

  SkRect textBounds;
  font.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &textBounds);
  font_ = font;
  text_bounds_ = textBounds;

  SkFontMetrics fontMetrics;
  font_.getMetrics(&fontMetrics);

  text_metrics_.ascent = -fontMetrics.fAscent;
  text_metrics_.descent = fontMetrics.fDescent;
  text_metrics_.leading = fontMetrics.fLeading;
  text_metrics_.x_advance = textBounds.fRight - textBounds.fLeft;

  bounds_.width = text_metrics_.x_advance;
  bounds_.height = std::max(0.0f, -textBounds.fTop) + std::max(0.0f, textBounds.fBottom - text_metrics_.descent) +
                   fontMetrics.fDescent;

  text_bounds_offset_x_ = textBounds.fLeft;
  text_bounds_offset_y_ = textBounds.fTop;
}

void TextRenderer::draw(SkCanvas *canvas) {
  // SkRect textBounds;
  // font_.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &textBounds);

  const SkRect &textBounds = text_bounds_;

  float centerX = bounds_.x + bounds_.width / 2.0f;
  float centerY = bounds_.y + bounds_.height / 2.0f;

  float drawX = centerX - textBounds.centerX();
  float drawY = centerY - textBounds.centerY();

  // float drawX = bounds_.x - text_bounds_offset_x_;
  // float drawY = bounds_.y - text_bounds_offset_y_;

  canvas->drawSimpleText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, drawX, drawY, font_, paint_);
  UIComponent::draw(canvas);
}

void TextRenderer::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "text: " << fmt::format("\"{}\"", text_) << "\n";
  os << pad << "style: " << style_.toString() << "\n";
}
