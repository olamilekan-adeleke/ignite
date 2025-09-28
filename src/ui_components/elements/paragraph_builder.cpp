#include <modules/skparagraph/include/ParagraphBuilder.h>
#include "elements/paragraph_builder.hpp"
#include "debug_assert.hpp"
#include "size.hpp"
#include "ui_manager.hpp"

ParagraphBuilder::ParagraphBuilder(const std::string& text, const TextStyle& params) : text_(text), params_(params) {
  fontCollection_ = sk_make_sp<skia::textlayout::FontCollection>();
  const sk_sp<SkFontMgr>& fontManager = UIManager::instance().fontManager();
  fontCollection_->setDefaultFontManager(fontManager);
  fontCollection_->enableFontFallback();
  buildParagraph();
}

UISize ParagraphBuilder::getIntrinsicSize(UIConstraints constraints) noexcept {
  float layoutWidth = constraints.minWidth > 0 ? constraints.minWidth : 1000000.0f;

  if (!paragraph_) buildParagraph();
  paragraph_->layout(layoutWidth);

  float paragraphMaxWidth = std::min(paragraph_->getMaxWidth(), paragraph_->getMaxIntrinsicWidth());
  float paragraphHeight = paragraph_->getHeight();

  float usedWidth = std::min(constraints.minWidth, paragraphMaxWidth);
  float usedHeight = paragraphHeight;

  VERIFY(paragraphMaxWidth > layoutWidth * 1.1f,
         fmt::format("  - WARNING: getMaxWidth({}) >> layoutWidth({}), paragraph layout may have failed!",
                     paragraphMaxWidth,
                     layoutWidth));

  return UISize{.width = usedWidth, .height = usedHeight};
}

void ParagraphBuilder::draw(SkCanvas* canvas, SkPoint point) {
  canvas->save();
  canvas->translate(point.x(), point.y());
  paragraph_->paint(canvas, 0, 0);
  canvas->restore();
}

void ParagraphBuilder::buildParagraph() {
  skia::textlayout::ParagraphStyle paragraphStyle;
  paragraphStyle.setTextAlign(params_.textAlign);
  if (params_.maxLines > 0) {
    paragraphStyle.setMaxLines(params_.maxLines);
    paragraphStyle.setEllipsis(u"...");
  }

  skia::textlayout::TextStyle textStyle;
  textStyle.setColor(params_.color);
  textStyle.setFontSize(params_.fontSize);

  std::unique_ptr<skia::textlayout::ParagraphBuilder> builder =
      skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection_);

  builder->pushStyle(textStyle);
  const std::u16string text = utf8_to_utf16(text_);
  builder->addText(text);
  builder->pop();

  paragraph_ = builder->Build();
}
