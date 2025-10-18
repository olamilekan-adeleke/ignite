#include "elements/paragraph_builder.hpp"

#include <fmt/base.h>
#include <modules/skparagraph/include/ParagraphBuilder.h>

#include "foundation/foundation.hpp"
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
  if (!paragraph_) buildParagraph();

  const float desiredWidth = paragraph_->getMaxIntrinsicWidth() + 1.0f;
  const float layoutWidth = std::clamp(desiredWidth, constraints.minWidth, constraints.maxWidth);

  paragraph_->layout(layoutWidth);

  float layoutHeight = paragraph_->getHeight();
  return UISize{.width = layoutWidth, .height = layoutHeight};
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
