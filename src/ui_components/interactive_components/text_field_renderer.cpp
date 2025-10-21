#include "interactive_components/text_field_renderer.hpp"

#include <fmt/base.h>
#include <include/core/SkRRect.h>

#include <algorithm>

#include "foundation/foundation.hpp"
#include "ui_manager.hpp"

void TextFieldRenderer::onTextFieldTap() noexcept { UIManager::instance().requestFocus(*this); }

// void TextFieldRenderer::layout(UIConstraints constraints) {
//   float contentWidth = 0.0f;
//
//   if (params_.size.width > 0.0f) {
//     contentWidth = params_.size.width - params_.padding.horizonal();
//   } else {
//     contentWidth = constraints.maxWidth - params_.padding.horizonal();
//   }
//
//   UIConstraints contentConstraints{
//       .minWidth = 0.0f,
//       .maxWidth = constraints.maxWidth - params_.padding.horizonal(),
//   };
//
//   const UISize& placeHolderSize = placeholderParagraph_.getIntrinsicSize(contentConstraints);
//   const UISize& valueSize = textValueParagraph_.getIntrinsicSize(contentConstraints);
//
//   float finalWidth = std::max(placeHolderSize.width, valueSize.width) + params_.padding.horizonal();
//   contentConstraints.minWidth = finalWidth;
//
//   float contentHeight;
//   if (params_.multiline) {
//     contentHeight = valueSize.height;
//   } else {
//     contentHeight = std::max(params_.size.height, std::max(placeHolderSize.height, valueSize.height));
//   }
//
//   lastTextFieldHeight_ = std::max(placeHolderSize.height, valueSize.height);
//   setSize(constraints.maxWidth, contentHeight + params_.padding.vertical());
// }

void TextFieldRenderer::layout(UIConstraints constraints) {
  float contentWidth = 0.0f;

  if (params_.size.width > 0.0f) {
    contentWidth = params_.size.width - params_.padding.horizonal();
  } else {
    contentWidth = constraints.maxWidth - params_.padding.horizonal();
  }

  // Create content constraints with proper width
  UIConstraints contentConstraints{
      .minWidth = contentWidth,  // Set minWidth to prevent wrapping
      .maxWidth = contentWidth,  // Match maxWidth to minWidth for single-line
  };

  // If multiline, allow flexible width
  if (params_.multiline) {
    contentConstraints.minWidth = 0.0f;
    contentConstraints.maxWidth = constraints.maxWidth - params_.padding.horizonal();
  }

  const UISize& placeHolderSize = placeholderParagraph_.getIntrinsicSize(contentConstraints);
  const UISize& valueSize = textValueParagraph_.getIntrinsicSize(contentConstraints);

  float contentHeight;
  if (params_.multiline) {
    contentHeight = valueSize.height;
  } else {
    contentHeight = std::max(params_.size.height, std::max(placeHolderSize.height, valueSize.height));
  }

  lastTextFieldHeight_ = std::max(placeHolderSize.height, valueSize.height);

  // Use the actual content width (not max from constraints)
  float finalWidth = params_.size.width > 0.0f
                         ? params_.size.width
                         : std::max(placeHolderSize.width, valueSize.width) + params_.padding.horizonal();

  setSize(finalWidth, contentHeight + params_.padding.vertical());
}

void TextFieldRenderer::draw(SkCanvas* canvas) {
  SkPaint backgroundPaint;
  backgroundPaint.setColor(params_.backgroundColor);
  backgroundPaint.setStyle(SkPaint::kFill_Style);
  backgroundPaint.setAntiAlias(true);

  SkPaint borderPaint;
  borderPaint.setColor(params_.borderColor);
  borderPaint.setStyle(SkPaint::kStroke_Style);
  borderPaint.setAntiAlias(true);
  borderPaint.setStrokeJoin(SkPaint::kRound_Join);
  borderPaint.setStrokeWidth(1.0f);

  SkRect rect = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
  SkRRect rrect;
  rrect.setRectXY(rect, params_.radius, params_.radius);
  if (params_.radius > 0) {
    canvas->drawRRect(rrect, backgroundPaint);
    canvas->drawRRect(rrect, borderPaint);
  } else {
    canvas->drawRect(rect, backgroundPaint);
    canvas->drawRect(rect, borderPaint);
  }

  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  // Draw the paragraph text
  const float paragraphHeight = lastTextFieldHeight_;
  const float offsetY = std::round((bounds_.height - paragraphHeight) / 2.0f + 0.5f);
  const float offsetX = params_.padding.left;

  // SkRect clipRect = SkRect::MakeXYWH(textFieldX, textFieldY, textFieldWidth, textFieldHeight);
  // canvas->clipRect(clipRect);

  if (!buffer_.empty()) {
    textValueParagraph_.draw(canvas, SkPoint::Make(offsetX, offsetY));
  } else {
    placeholderParagraph_.draw(canvas, SkPoint::Make(offsetX, offsetY));
  }

  // Draw cursor
  auto currentTime = std::chrono::steady_clock::now();
  auto timeSinceLastBlink = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastBlinkTime_).count();

  if (timeSinceLastBlink >= CURSOR_BLINK_INTERVAL_MS) {
    cursorVisible_ = !cursorVisible_;
    lastBlinkTime_ = currentTime;
  }

  if (cursorVisible_ && hasFocus()) {
    SkPaint paint;
    paint.setColor(SK_ColorBLACK);
    paint.setStrokeWidth(1.5f);

    float cursorX = offsetX;
    std::vector<skia::textlayout::TextBox> rects{};

    if (buffer_.empty() || cursorIndex_ == 0) {
      cursorX = offsetX;
    } else if (cursorIndex_ >= buffer_.size()) {
      rects = textValueParagraph_.getRectsForRange(buffer_.size() - 1,
                                                   buffer_.size(),
                                                   skia::textlayout::RectHeightStyle::kTight,
                                                   skia::textlayout::RectWidthStyle::kMax);

      if (!rects.empty()) cursorX = rects.front().rect.fRight + offsetX;
    } else {
      // Middle of text: get rect of character at cursor position
      rects = textValueParagraph_.getRectsForRange(cursorIndex_,
                                                   cursorIndex_ + 1,
                                                   skia::textlayout::RectHeightStyle::kTight,
                                                   skia::textlayout::RectWidthStyle::kMax);
      if (!rects.empty()) cursorX = rects.front().rect.fLeft + offsetX;
    }

    float cursorTop = offsetY;
    float cursorBottom = offsetY + paragraphHeight;

    if (!rects.empty()) {
      cursorTop = rects.front().rect.fTop + offsetY;
      cursorBottom = rects.front().rect.fBottom + offsetY;
    }

    canvas->drawLine(cursorX, cursorTop, cursorX, cursorBottom, paint);
  }

  canvas->restore();

  UIComponent::draw(canvas);
}
