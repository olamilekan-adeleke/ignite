#pragma once

#include <string>

#include "foundation/foundation.hpp"
#include "include/core/SkCanvas.h"
#include "include/core/SkPoint.h"
#include "include/core/SkSurface.h"
#include "modules/skparagraph/include/FontCollection.h"
#include "modules/skparagraph/include/Paragraph.h"

class ParagraphBuilder {
 public:
  ParagraphBuilder(const std::string& text, const TextStyle& params = {});

  void draw(SkCanvas* canvas, SkPoint point);
  void layout(float width);

  UISize getIntrinsicSize(UIConstraints constraints) noexcept;

  void debugFillProperties(std::ostringstream& os, int indent) const;

  std::vector<skia::textlayout::TextBox> getRectsForRange(unsigned start,
                                                          unsigned end,
                                                          skia::textlayout::RectHeightStyle rectHeightStyle,
                                                          skia::textlayout::RectWidthStyle rectWidthStyle);

  void setText(const std::string& newText);

 private:
  std::string text_;
  TextStyle params_;
  sk_sp<skia::textlayout::FontCollection> fontCollection_;
  std::unique_ptr<skia::textlayout::Paragraph> paragraph_;

  void buildParagraph();
};

inline void ParagraphBuilder::debugFillProperties(std::ostringstream& os, int indent) const {
  std::string pad(indent, ' ');
}

inline std::vector<skia::textlayout::TextBox> ParagraphBuilder::getRectsForRange(
    unsigned start,
    unsigned end,
    skia::textlayout::RectHeightStyle rectHeightStyle,
    skia::textlayout::RectWidthStyle rectWidthStyle) {
  return paragraph_->getRectsForRange(start, end, rectHeightStyle, rectWidthStyle);
}
