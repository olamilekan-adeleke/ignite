#include "elements/icon_renderer.hpp"

#include <fmt/base.h>

#include "include/core/SkFontMgr.h"
#include "include/core/SkTypeface.h"
#include "ui_manager.hpp"

void IconRenderObject::performLayout(UIConstraints size) noexcept { setSize(params_.size, params_.size); }

void IconRenderObject::paint(SkCanvas* canvas) noexcept {
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

    const auto& bounds = getBounds();
    float x = bounds.x;
    float y = bounds.y - metrics.fAscent;

    canvas->drawSimpleText(&glyphId, sizeof(SkGlyphID), SkTextEncoding::kGlyphID, x, y, font, paint);
  } else {
    fmt::println("Cannot draw - glyph not found! \"{}\"", params_.icon.unicode);
  }

  RenderObject::paint(canvas);
}
