#include "lib/user.hpp"

#include <include/core/SkFontMgr.h>

#include "include/core/SkTypeface.h"
#include "logger.hpp"
#include "skia/SkiaRenderer.hpp"

// NEW INCLUDE FOR CORETEXT FONT MANAGER
#if defined(SK_BUILD_FOR_MAC)  // Or check if SK_FONTMGR_CORETEXT_AVAILABLE is defined
#include "include/ports/SkFontMgr_mac_ct.h"
#endif

UserPage::UserPage(SkiaRenderer* renderer) : skiaRenderer(renderer) { setupPaint(); }

UserPage::~UserPage() {}

void UserPage::setupPaint() {
  // Setup text paint
  textPaint.setAntiAlias(true);
  textPaint.setColor(SK_ColorBLACK);

  // Setup font
  font.setSize(48);
  sk_sp<SkFontMgr> fontMgr;

#if defined(SK_BUILD_FOR_MAC)
  fontMgr = SkFontMgr_New_CoreText(nullptr);
#else
  fontMgr = SkFontMgr::RefEmpty();
  Logger::log("Warning: No platform-specific font manager available or configured.");
#endif

  if (fontMgr) {
    // Try to match a default family (nullptr) or a common one like "Helvetica"
    sk_sp<SkTypeface> defaultTypeface = fontMgr->matchFamilyStyle(nullptr, SkFontStyle::Normal());

    if (defaultTypeface) {
      font.setTypeface(std::move(defaultTypeface));
    } else {
      Logger::log("Warning: Could not get default typeface. Attempting 'Helvetica'.");
      defaultTypeface = fontMgr->matchFamilyStyle("Helvetica", SkFontStyle::Normal());
      if (defaultTypeface) {
        font.setTypeface(std::move(defaultTypeface));
      } else {
        font.setTypeface(SkTypeface::MakeEmpty());
        Logger::log("ERROR: Failed to get any usable typeface. Text will not be displayed.");
      }
    }
  } else {
    font.setTypeface(SkTypeface::MakeEmpty());
    Logger::log("ERROR: Failed to get any SkFontMgr. Text will not be displayed.");
  }
}

void UserPage::draw() {
  SkCanvas* canvas = skiaRenderer->getCanvas();
  if (!canvas) return;

  // Draw "Hello World" text
  const char* text = "Hello World";
  SkScalar x = 100;
  SkScalar y = 200;

  canvas->drawString(text, x, y, font, textPaint);

  // You can add more drawing code here
  // For example, drawing shapes:
  SkPaint circlePaint;
  circlePaint.setAntiAlias(true);
  circlePaint.setColor(SK_ColorBLUE);
  circlePaint.setStyle(SkPaint::kFill_Style);
  circlePaint.setStrokeWidth(3);

  canvas->drawCircle(300, 300, 50, circlePaint);
}
