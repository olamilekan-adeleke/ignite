#include "elements/paragraph_builder.hpp"

#include <fmt/base.h>
#include <modules/skparagraph/include/ParagraphBuilder.h>

#include "rect.hpp"
#include "size.hpp"
#include "ui_manager.hpp"

ParagraphBuilder::ParagraphBuilder(const std::string& text, const TextStyle& params) : text_(text), params_(params) {
  fontCollection_ = sk_make_sp<skia::textlayout::FontCollection>();
  const sk_sp<SkFontMgr>& fontManager = UIManager::instance().fontManager();
  fontCollection_->setDefaultFontManager(fontManager);
  fontCollection_->enableFontFallback();
  buildParagraph();
}

void ParagraphBuilder::setText(const std::string& newText) {
  if (text_ != newText) {
    text_ = newText;
    paragraph_.reset();
    buildParagraph();
  }
}

UISize ParagraphBuilder::getIntrinsicSize(UIConstraints constraints) noexcept {
  float layoutWidth = constraints.minWidth > 0 ? constraints.minWidth : 1000000.0f;
  if (!paragraph_) buildParagraph();

  paragraph_->layout(layoutWidth);

  float usedWidth = std::min(constraints.minWidth, paragraph_->getMaxIntrinsicWidth());
  usedWidth += 1.0f;
  usedWidth = std::min(usedWidth, constraints.minWidth);
  float usedHeight = paragraph_->getHeight();

  return UISize{.width = usedWidth, .height = usedHeight};
}

void ParagraphBuilder::layout(float width) {
  if (!paragraph_) buildParagraph();
  paragraph_->layout(width);
}

void ParagraphBuilder::draw(SkCanvas* canvas, SkPoint point) {
  canvas->save();
  canvas->translate(point.x(), point.y());
  paragraph_->paint(canvas, 0, 0);
  canvas->restore();
}

void ParagraphBuilder::buildParagraph() {
  skia::textlayout::ParagraphStyle paragraphStyle;

  paragraphStyle.setTextAlign(params_.textAlignment);
  if (params_.maxLines > 0) {
    paragraphStyle.setMaxLines(params_.maxLines);
    paragraphStyle.setEllipsis(u"...");
  }

  skia::textlayout::TextStyle textStyle;
  textStyle.setColor(params_.color);
  textStyle.setFontSize(params_.fontSize);
  textStyle.setFontStyle(params_.weight);
  textStyle.setDecorationColor(params_.color);
  textStyle.setDecoration(params_.decoration);

  std::unique_ptr<skia::textlayout::ParagraphBuilder> builder =
      skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection_);

  builder->pushStyle(textStyle);
  builder->addText(text_.c_str());
  builder->pop();

  paragraph_ = builder->Build();
}
