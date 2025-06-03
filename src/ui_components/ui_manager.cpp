#include "ui_components/ui_manager.hpp"

#include "core/logger.hpp"

#if defined(SK_BUILD_FOR_MAC)
#include <include/ports/SkFontMgr_mac_ct.h>
#endif

UIManager& UIManager::instance() {
  static UIManager instance;
  return instance;
}

UIManager::UIManager() {
#if defined(SK_BUILD_FOR_MAC)
  fontMgr_ = SkFontMgr_New_CoreText(nullptr);
#else
  fontMgr_ = SkFontMgr::RefEmpty();
  Logger::log("Warning: No platform-specific font manager available or configured.");
#endif

  if (fontMgr_) {
    defaultTypeface_ = fontMgr_->matchFamilyStyle(nullptr, SkFontStyle::Normal());

    if (defaultTypeface_) {
      font_.setTypeface(defaultTypeface_);
    } else {
      Logger::log("Warning: Could not get default typeface. Attempting 'Helvetica'.");
      defaultTypeface_ = fontMgr_->matchFamilyStyle("Helvetica", SkFontStyle::Normal());

      if (defaultTypeface_) {
        font_.setTypeface(defaultTypeface_);
      } else {
        Logger::log("ERROR: Failed to get any usable typeface. Text will not be displayed.");
        defaultTypeface_ = SkTypeface::MakeEmpty();
        font_.setTypeface(defaultTypeface_);
      }
    }
  } else {
    Logger::log("ERROR: Failed to get any SkFontMgr. Text will not be displayed.");
    defaultTypeface_ = SkTypeface::MakeEmpty();
    font_.setTypeface(defaultTypeface_);
  }
}

const SkFont& UIManager::defaultFont() const { return font_; }

const sk_sp<SkTypeface>& UIManager::typeface() const { return defaultTypeface_; }
