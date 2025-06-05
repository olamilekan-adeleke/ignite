#include "ui_components/text_component.hpp"

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontTypes.h>
#include <include/core/SkRect.h>
#include <include/core/SkTextBlob.h>

#include "ui_components/ui_manager.hpp"

TextComponent::TextComponent(const std::string& text, const TextStyle& style) : text_(text), style_(style) {
  paint_.setColor(style_.color);
  paint_.setAntiAlias(true);
}

void TextComponent::layout(float parentWidth, float parentHeight) {
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

  // float scaleFactor = std::min(parentWidth / referenceWidth_, parentHeight / referenceHeight_);
  //  float scaledFontSize = baseFontSize_ * scaleFactor;

  font.setSize(style_.fontSize);
  // font.fontSize_ = style_.fontSize;

  SkRect textBounds;
  font.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &textBounds);
  font_ = font;

  // Calculate and store real text metrics
  SkFontMetrics fontMetrics;
  font_.getMetrics(&fontMetrics);

  text_metrics_.ascent = -fontMetrics.fAscent;
  text_metrics_.descent = fontMetrics.fDescent;
  text_metrics_.leading = fontMetrics.fLeading;
  text_metrics_.x_advance = textBounds.fRight - textBounds.fLeft;

  // bounds_.width = text_metrics_.x_advance;
  // bounds_.height = text_metrics_.ascent + text_metrics_.descent;

  // bounds_.width = textBounds.width();
  // bounds_.height = textBounds.height();

  bounds_.width = text_metrics_.x_advance;
  bounds_.height = std::max(0.0f, -textBounds.fTop) + std::max(0.0f, textBounds.fBottom - text_metrics_.descent) +
                   fontMetrics.fDescent;

  text_bounds_offset_x_ = textBounds.fLeft;
  text_bounds_offset_y_ = textBounds.fTop;

  // int skWeight = SkFontStyle::kNormal_Weight;
  // if (style_.weight == FontWeight::Bold) {
  //   skWeight = SkFontStyle::kBold_Weight;
  // } else if (style_.weight == FontWeight::Light) {
  //   skWeight = SkFontStyle::kLight_Weight;
  // }
  //
  // int slant = style_.italic ? SkFontStyle::kItalic_Slant : SkFontStyle::kUpright_Slant;
  // SkFontStyle style(skWeight, SkFontStyle::kNormal_Width, (SkFontStyle::Slant)slant);
  //
  // SkFont font = UIManager::instance().defaultFont();
  // sk_sp<SkTypeface> typeface = UIManager::instance().fontManager()->matchFamilyStyle(nullptr, style);
  // font.setTypeface(typeface ? std::move(typeface) : SkTypeface::MakeEmpty());
  // font.setSize(style_.fontSize);
  //
  // SkRect textBounds;
  // font.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &textBounds);
  //
  // font_ = font;
  //
  // bounds_.width = textBounds.fRight - textBounds.fLeft;
  // bounds_.height = textBounds.fBottom - textBounds.fTop;
}

void TextComponent::draw(SkCanvas* canvas) {
  float drawX = bounds_.x - text_bounds_offset_x_;
  float drawY = bounds_.y - text_bounds_offset_y_;

  // canvas->drawSimpleText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, bounds_.x, bounds_.y +
  // style_.fontSize,
  //                        font_, paint_);

  canvas->drawSimpleText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, drawX, drawY, font_, paint_);

  // canvas->drawString(text_.c_str(), drawX, drawY, font_, paint_);
  UIComponent::draw(canvas);
}
