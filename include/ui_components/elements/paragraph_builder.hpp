#pragma once

#include <string>
#include "size.hpp"
#include "include/core/SkSurface.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkPoint.h"
#include "modules/skparagraph/include/FontCollection.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "text_style.hpp"

class ParagraphBuilder {
 public:
  ParagraphBuilder(const std::string& text, const TextStyle& params = {});

  void draw(SkCanvas* canvas, SkPoint point);
  void layout(float width);

  UISize getIntrinsicSize(UIConstraints constraints) noexcept;

  void debugFillProperties(std::ostringstream& os, int indent) const;

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
