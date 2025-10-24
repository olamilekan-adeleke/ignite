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
    lastLayoutWidth_ = -1.0f;
    paragraph_.reset();
    buildParagraph();
  }
}

void ParagraphBuilder::setStyle(const TextStyle& newStyle) {
  if (params_ != newStyle) {
    params_ = newStyle;
    lastLayoutWidth_ = -1.0f;
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

float ParagraphBuilder::getHeight() const {
  if (!paragraph_) return 0.0f;
  return paragraph_->getHeight();
}

float ParagraphBuilder::getWidth() const {
  if (!paragraph_) return 0.0f;
  if (lastLayoutWidth_ == -1.0f) {
    return paragraph_->getMaxIntrinsicWidth() + 1.0f;
  }
  return lastLayoutWidth_;
}

void ParagraphBuilder::layout(float width) {
  if (!paragraph_) buildParagraph();
  if (!paragraph_) return;

  // Skip redundant layout calls - only layout if width actually changed
  if (lastLayoutWidth_ != width) {
    paragraph_->layout(width);
    lastLayoutWidth_ = paragraph_->getMaxIntrinsicWidth() + 1.0f;
  }
}

void ParagraphBuilder::draw(SkCanvas* canvas, SkPoint point) {
  if (!paragraph_) return;

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
