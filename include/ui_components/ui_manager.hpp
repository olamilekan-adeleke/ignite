#pragma once

#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkTypeface.h>

#include <memory>

#include "basic/ui_component.hpp"
#include "tap_event.hpp"

namespace Debug {
inline bool ui_debug_mode = false;
}

// just to maanger some UI Shit
class UIManager {
 public:
  UIManager();
  static UIManager &instance();

  void setTree(const std::shared_ptr<UIComponent> tree, float w, float h, bool needsResize);
  void diffAndRebuild(const std::shared_ptr<UIComponent> &oldNode,
                      const std::shared_ptr<UIComponent> &newNode,
                      float w,
                      float h,
                      bool needsResize);

  const SkFont &defaultFont() const;
  const sk_sp<SkTypeface> &typeface() const;
  const sk_sp<SkFontMgr> &fontManager() const;

  void sendTapEvent(const UITapEvent &event);

 private:
  sk_sp<SkFontMgr> fontMgr_;
  sk_sp<SkTypeface> defaultTypeface_;
  SkFont font_;

  std::shared_ptr<UIComponent> currentTreeRoot_ = nullptr;
  std::shared_ptr<UIComponent> previousTreeRoot_ = nullptr;
  float width_ = 0;
  float height_ = 0;
  bool dirty_ = false;

  bool shouldRebuild(const std::shared_ptr<UIComponent> &oldNode, const std::shared_ptr<UIComponent> &newNode);
};
