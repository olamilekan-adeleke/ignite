#pragma once

#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkImage.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTypeface.h>

#include <filesystem>
#include <iostream>
#include <memory>

#include "basic/ui_component.hpp"
#include "include/core/SkFontMgr.h"
#include "include/core/SkTypeface.h"
#include "offset.hpp"
#include "rect.hpp"
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
  void setScrollCallback(const Offset &offset);

  void sendKeyEvent(int key, int scancode, int action, int mods);
  void sendMouseEvent(double xpos, double ypos);
  void sendCharEvent(unsigned int codepoint);

  void requestFocus(UIComponent &component);
  void releaseFocus(UIComponent &component);
  void releaseAllFocus(std::optional<std::shared_ptr<UIComponent>> component = std::nullopt);

 private:
  sk_sp<SkFontMgr> fontMgr_;
  sk_sp<SkTypeface> defaultTypeface_;
  SkFont font_;

  std::shared_ptr<UIComponent> currentTreeRoot_ = nullptr;
  std::shared_ptr<UIComponent> previousTreeRoot_ = nullptr;
  float width_ = 0;
  float height_ = 0;
  bool dirty_ = false;

  std::shared_ptr<UIComponent> currentHoveredComponent_ = nullptr;
  std::shared_ptr<UIComponent> currentFocusedComponent_ = nullptr;
};

class UICacheManager {
 public:
  static UICacheManager &instance() {
    static UICacheManager instance;
    return instance;
  }

  UICacheManager(const UICacheManager &) = delete;
  void operator=(const UICacheManager &) = delete;

  sk_sp<SkSurface> getCachedSurface(const uint64_t drawHash) {
    auto it = cache_.find(drawHash);
    if (it != cache_.end()) return it->second;
    return nullptr;
  }

  std::optional<UIRect> getLayoutCached(const uint64_t layoutHash) {
    auto it = layoutCache_.find(layoutHash);
    if (it != layoutCache_.end()) return it->second;
    return std::nullopt;
  }

  void setCachedSurface(const uint64_t drawHash, sk_sp<SkSurface> surface) {
    if (surface) cache_[drawHash] = surface;
  }

  void setLayoutCached(const uint64_t layoutHash, const UIRect &rect) { layoutCache_[layoutHash] = rect; }

  void removeCachedSurface(const uint64_t drawHash) { cache_.erase(drawHash); }

  void removeLayoutCached(const uint64_t layoutHash) { layoutCache_.erase(layoutHash); }

  void clear() {
    cache_.clear();
    layoutCache_.clear();
  }

 private:
  UICacheManager() = default;

  std::unordered_map<uint64_t, sk_sp<SkSurface>> cache_;
  std::unordered_map<uint64_t, UIRect> layoutCache_;
};

class UIFontManager {
 public:
  static UIFontManager &instance() {
    static UIFontManager instance;
    return instance;
  }

  UIFontManager(const UIFontManager &) = delete;
  void operator=(const UIFontManager &) = delete;

  sk_sp<SkTypeface> getTypeface(const std::string &fontPath = "icon_pack/Material_Icons_Regular.ttf") {
    std::filesystem::path fullPath = std::filesystem::current_path() / fontPath;

    auto it = cache_.find(fontPath);
    if (it != cache_.end()) {
      // std::cout << "[UIFontManager] Font found in cache: " << fontPath << "\n";
      return it->second;
    }

    // Use real font manager
    auto fontMgr = UIManager::instance().fontManager();
    sk_sp<SkTypeface> typeface = fontMgr->makeFromFile(fullPath.string().c_str());

    if (!typeface) {
      std::cout << "[UIFontManager] Failed to load font from: " << fullPath << "\n";
    } else {
      std::cout << "[UIFontManager] Successfully loaded font: " << fontPath << "\n";
      cache_[fontPath] = typeface;
    }

    return typeface;
  }

  void removeTypeface(const std::string &fontPath) { cache_.erase(fontPath); }

  void clear() { cache_.clear(); }

 private:
  UIFontManager() = default;

  std::unordered_map<std::string, sk_sp<SkTypeface>> cache_;
};
