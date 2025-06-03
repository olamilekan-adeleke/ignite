#include "ui_components/text_component.hpp"

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontTypes.h>

#include "ui_components/ui_manager.hpp"

TextComponent::TextComponent(const std::string& text, Color color, float fontSize, FontWeight weight, bool underline,
                             bool italic)
    : text_(text), color_(color), fontSize_(fontSize), weight_(weight), underline_(underline), italic_(italic) {
  paint_.setColor(color_);
  paint_.setAntiAlias(true);
}

TextComponent::TextComponent(const std::string& text, float fontSize, Color color)
    : TextComponent(text, color, fontSize) {}

TextComponent::TextComponent(const std::string& text, float fontSize, FontWeight weight)
    : TextComponent(text, Color::Black(), fontSize, weight) {}

TextComponent::TextComponent(const std::string& text, float fontSize) : TextComponent(text, Color::Black(), fontSize) {}

TextComponent::TextComponent(const std::string& text, Color color) : TextComponent(text, color, 16.0f) {}

TextComponent::TextComponent(const std::string& text, float fontSize, FontWeight weight, bool underline, bool italic)
    : TextComponent(text, Color::Black(), fontSize, weight, underline, italic) {}

void TextComponent::layout(float parentWidth, float parentHeight) {
  int skWeight = SkFontStyle::kNormal_Weight;
  if (weight_ == FontWeight::Bold)
    skWeight = SkFontStyle::kBold_Weight;
  else if (weight_ == FontWeight::Light)
    skWeight = SkFontStyle::kLight_Weight;

  int slant = italic_ ? SkFontStyle::kItalic_Slant : SkFontStyle::kUpright_Slant;
  SkFontStyle style(skWeight, SkFontStyle::kNormal_Width, (SkFontStyle::Slant)slant);

  SkFont font = UIManager::instance().defaultFont();
  sk_sp<SkTypeface> typeface = UIManager::instance().fontManager()->matchFamilyStyle(nullptr, style);
  font.setTypeface(typeface ? std::move(typeface) : SkTypeface::MakeEmpty());
  font.setSize(fontSize_);

  SkRect textBounds;
  font.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &textBounds);

  font_ = font;

  bounds_.width = textBounds.width();
  bounds_.height = textBounds.height();
}

void TextComponent::draw(SkCanvas* canvas) {
  canvas->drawSimpleText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, bounds_.x, bounds_.y + fontSize_, font_,
                         paint_);
}
