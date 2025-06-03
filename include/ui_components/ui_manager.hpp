#pragma once

#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkTypeface.h>

// just to maanger some UI Shit
class UIManager {
 public:
  UIManager();
  static UIManager& instance();

  const SkFont& defaultFont() const;
  const sk_sp<SkTypeface>& typeface() const;

 private:
  sk_sp<SkFontMgr> fontMgr_;
  sk_sp<SkTypeface> defaultTypeface_;
  SkFont font_;
};
