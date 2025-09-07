#include "ui_manager.hpp"

#include <fmt/base.h>
#include <algorithm>
#include <memory>
#include <string>

#include "core/logger.hpp"
#include "tap_event.hpp"

#if defined(SK_BUILD_FOR_MAC)
#include <include/ports/SkFontMgr_mac_ct.h>
#endif

UIManager &UIManager::instance() {
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
        Logger::log(
            "ERROR: Failed to get any usable typeface. Text will not "
            "be displayed.");
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

const SkFont &UIManager::defaultFont() const { return font_; }

const sk_sp<SkTypeface> &UIManager::typeface() const { return defaultTypeface_; }

const sk_sp<SkFontMgr> &UIManager::fontManager() const { return fontMgr_; }

void UIManager::sendTapEvent(const UITapEvent &event) {
  if (currentTreeRoot_) {
    bool handled = currentTreeRoot_->processTap(event);
    if (handled) {
      fmt::println("Tap handled by UI at ({}, {})", event.x, event.y);
    } else {
      fmt::println("Tap not handled at ({}, {})", event.x, event.y);
    }
  }
}

void UIManager::setTree(const std::shared_ptr<UIComponent> tree, float w, float h, bool needsResize) {
  currentTreeRoot_ = tree;
  width_ = w;
  height_ = h;

  diffAndRebuild(previousTreeRoot_, currentTreeRoot_, w, h, needsResize);
  previousTreeRoot_ = currentTreeRoot_;
}

// TODO: implement better diffing
void UIManager::diffAndRebuild(const std::shared_ptr<UIComponent> &oldNode,
                               const std::shared_ptr<UIComponent> &newNode,
                               float w,
                               float h,
                               bool needsResize) {
  bool rebuild = true;  // shouldRebuild(oldNode, newNode);
  if (rebuild || needsResize) {
    // if (oldNode) {
    //   fmt::println("Checking for rebuild: {} == {}", newNode->key().value(), oldNode->key().value());
    // } else {
    //   fmt::println("Checking for rebuild: {} == <null oldNode>", newNode->key().value());
    // }
    newNode->layout({w, h});
  }

  auto oldChildren = oldNode ? oldNode->children() : std::vector<std::shared_ptr<UIComponent>>{};
  auto newChildren = newNode->children();

  for (size_t i = 0; i < newChildren.size(); ++i) {
    const auto &newChild = newChildren[i];
    const std::shared_ptr<UIComponent> oldChild = (i < oldChildren.size()) ? oldChildren[i] : nullptr;

    diffAndRebuild(oldChild, newChild, w, h, needsResize);
  }
}

bool UIManager::shouldRebuild(const std::shared_ptr<UIComponent> &oldNode,
                              const std::shared_ptr<UIComponent> &newNode) {
  if (!oldNode || !newNode) {
    return true;
  } else if (oldNode->key() != newNode->key()) {
    return true;
  }

  return false;
}
