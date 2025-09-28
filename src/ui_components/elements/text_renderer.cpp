#include "elements/text_renderer.hpp"
#include <fmt/base.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPoint.h>
#include <algorithm>
#include <sstream>
#include <string>
#include "elements/paragraph_builder.hpp"
#include "size.hpp"

TextRenderer::TextRenderer(const std::string &text, const TextStyle &style)
    : text_(text), paragraphBuilder_(ParagraphBuilder(text, style)) {}

UISize TextRenderer::getIntrinsicSize(UIConstraints constraints) noexcept {
  return paragraphBuilder_.getIntrinsicSize(constraints);
}

void TextRenderer::layout(UISize size) {
  UIConstraints constraints = {size.width, size.height};
  UISize intrinsicSize = paragraphBuilder_.getIntrinsicSize(constraints);

  bounds_.width = intrinsicSize.width;
  bounds_.height = std::min(intrinsicSize.height, size.height);
}

void TextRenderer::draw(SkCanvas *canvas) {
  canvas->save();
  SkRect clipRect = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
  canvas->clipRect(clipRect);

  paragraphBuilder_.draw(canvas, SkPoint::Make(bounds_.x, bounds_.y));
  canvas->restore();

  UIComponent::draw(canvas);
}

void TextRenderer::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  paragraphBuilder_.debugFillProperties(os, indent);
  os << pad << "text: " << fmt::format("\"{}\"", text_) << "\n";
}
