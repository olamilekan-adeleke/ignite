#include "ui_components/text_component.hpp"

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontTypes.h>

#include "ui_components/ui_manager.hpp"

Text::Text(const std::string& text, Color color, float fontSize, FontWeight weight, bool underline, bool italic)
    : text_(text), color_(color), fontSize_(fontSize), weight_(weight), underline_(underline), italic_(italic) {
  paint_.setColor(color_);
  paint_.setAntiAlias(true);
}

Text::Text(const std::string& text, float fontSize, Color color) : Text(text, color, fontSize) {}

Text::Text(const std::string& text, float fontSize, FontWeight weight) : Text(text, Color::Black(), fontSize, weight) {}

Text::Text(const std::string& text, float fontSize) : Text(text, Color::Black(), fontSize) {}

Text::Text(const std::string& text, Color color) : Text(text, color, 16.0f) {}

Text::Text(const std::string& text, float fontSize, FontWeight weight, bool underline, bool italic)
    : Text(text, Color::Black(), fontSize, weight, underline, italic) {}

void Text::layout(float parentWidth, float parentHeight) {
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

  bounds_.width = textBounds.width();
  bounds_.height = textBounds.height();
}

void Text::draw(SkCanvas* canvas) {
  canvas->drawSimpleText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, bounds_.x, bounds_.y + fontSize_,
                         UIManager::instance().defaultFont(), paint_);
}
