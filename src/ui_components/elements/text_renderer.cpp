#include "elements/text_renderer.hpp"

#include <fmt/base.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPoint.h>

#include <sstream>
#include <string>

#include "elements/paragraph_builder.hpp"
#include "size.hpp"

TextRenderer::TextRenderer(const std::string &text, const TextStyle &style)
    : text_(std::move(text)), paragraphBuilder_(ParagraphBuilder(text, style)) {}

UISize TextRenderer::getIntrinsicSize(UIConstraints constraints) noexcept {
  auto size = paragraphBuilder_.getIntrinsicSize(constraints);
  return size;
}

void TextRenderer::layout(UIConstraints size) {
  auto childSize = paragraphBuilder_.getIntrinsicSize(size);
  paragraphBuilder_.layout(childSize.width);
  setSize(childSize);
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
