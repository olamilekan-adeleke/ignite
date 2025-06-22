#include "ui_components/ui_manager.hpp"

#include <fmt/base.h>
#include <memory>

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

/**
 * @brief Sets the current UI component tree and updates its layout.
 *
 * Replaces the existing UI tree root with a new one, updates the UI dimensions, and triggers a diff and rebuild process to update the UI layout as needed.
 *
 * @param tree The new root UI component tree.
 * @param w The width for the UI layout.
 * @param h The height for the UI layout.
 * @param needsResize Indicates whether a resize operation is required.
 */
void UIManager::setTree(const std::shared_ptr<UIComponent> tree, float w, float h, bool needsResize) {
  currentTreeRoot_ = tree;
  width_ = w;
  height_ = h;

  diffAndRebuild(previousTreeRoot_, currentTreeRoot_, w, h, needsResize);
  previousTreeRoot_ = currentTreeRoot_;
}

/**
 * @brief Recursively updates the UI component tree by comparing old and new nodes.
 *
 * If a node requires rebuilding or resizing, its layout is recalculated. The method then recursively processes all child components, ensuring the UI tree reflects the latest structure and dimensions.
 *
 * @param oldNode The previous UI component node, or nullptr if none exists.
 * @param newNode The updated UI component node.
 * @param w The width to use for layout calculations.
 * @param h The height to use for layout calculations.
 * @param needsResize Indicates whether a resize operation is required.
 */
void UIManager::diffAndRebuild(const std::shared_ptr<UIComponent> &oldNode,
                               const std::shared_ptr<UIComponent> &newNode,
                               float w,
                               float h,
                               bool needsResize) {
  bool rebuild = shouldRebuild(oldNode, newNode);
  if (needsResize || rebuild) {
    newNode->layout(w, h);
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
