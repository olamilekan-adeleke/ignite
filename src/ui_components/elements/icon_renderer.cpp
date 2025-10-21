#include "elements/icon_renderer.hpp"

#include <fmt/base.h>

#include "foundation/foundation.hpp"
#include "include/core/SkFontMgr.h"
#include "include/core/SkTypeface.h"
#include "ui_manager.hpp"

UISize IconRenderer::getIntrinsicSize(UIConstraints constraints) noexcept { return {params_.size, params_.size}; }

void IconRenderer::layout(UIConstraints size) { setSize(params_.size, params_.size); }

void IconRenderer::draw(SkCanvas* canvas) {
  sk_sp<SkTypeface> iconTypeface = UIFontManager::instance().getTypeface();
  if (!iconTypeface) return;

  SkFont font(iconTypeface, params_.size);
  SkPaint paint;
  paint.setColor(params_.color);
  paint.setAntiAlias(true);

  SkGlyphID glyphId = iconTypeface->unicharToGlyph(params_.icon.unicode);
  if (glyphId > 0) {
    SkFontMetrics metrics;
    font.getMetrics(&metrics);

    float x = bounds_.x;
    float y = bounds_.y - metrics.fAscent;

    canvas->drawSimpleText(&glyphId, sizeof(SkGlyphID), SkTextEncoding::kGlyphID, x, y, font, paint);
  } else {
    fmt::println("Cannot draw - glyph not found! \"{}\"", params_.icon.unicode);
  }

  UIComponent::draw(canvas);
}
