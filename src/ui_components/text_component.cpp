#include "ui_components/text_component.hpp"

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontTypes.h>

#include "ui_components/ui_manager.hpp"

Text::Text(const std::string& text, Color color, float fontSize) : text_(text), color_(color), fontSize_(fontSize) {
  paint_.setColor(color_);
  paint_.setAntiAlias(true);
}

void Text::layout(float parentWidth, float parentHeight) {
  SkFont font = UIManager::instance().defaultFont();
  SkRect textBounds;
  font.measureText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, &textBounds);

  bounds_.width = textBounds.width();
  bounds_.height = textBounds.height();
}

void Text::draw(SkCanvas* canvas) {
  canvas->drawSimpleText(text_.c_str(), text_.length(), SkTextEncoding::kUTF8, bounds_.x, bounds_.y + fontSize_,
                         UIManager::instance().defaultFont(), paint_);
}
