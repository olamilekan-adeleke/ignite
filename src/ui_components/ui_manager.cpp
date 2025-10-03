#include "ui_manager.hpp"

#include <fmt/base.h>

#include <memory>
#include <string>

#include "core/logger.hpp"
#include "keyboard_key_event.hpp"
#include "offset.hpp"
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
#elif defined(SK_BUILD_FOR_LINUX)
  fontMgr_ = SkFontMgr_New_DirectWrite();
#elif defined(SK_BUILD_FOR_WIN)
  fontMgr_ = SkFontMgr_New_DirectWrite();
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

void UIManager::setScrollCallback(const Offset &offset) {
  if (currentTreeRoot_) {
    currentTreeRoot_->setCursorPosCallback(offset);
  }
}

void UIManager::setTree(const std::shared_ptr<UIComponent> tree, float w, float h, bool needsResize) {
  currentTreeRoot_ = tree;
  width_ = w;
  height_ = h;

  currentTreeRoot_->updateGlobalOffset({0.0f, 0.0f});
  diffAndRebuild(previousTreeRoot_, currentTreeRoot_, w, h, needsResize);
  previousTreeRoot_ = currentTreeRoot_;
}

void UIManager::diffAndRebuild(const std::shared_ptr<UIComponent> &oldNode,
                               const std::shared_ptr<UIComponent> &newNode,
                               float w,
                               float h,
                               bool needsResize) {
  newNode->layout({w, h});

  auto oldChildren = oldNode ? oldNode->children() : std::vector<std::shared_ptr<UIComponent>>{};
  auto newChildren = newNode->children();

  for (size_t i = 0; i < newChildren.size(); ++i) {
    const auto &newChild = newChildren[i];
    const std::shared_ptr<UIComponent> oldChild = (i < oldChildren.size()) ? oldChildren[i] : nullptr;

    diffAndRebuild(oldChild, newChild, w, h, needsResize);
  }
}

void UIManager::sendKeyEvent(int key, int scancode, int action, int mods) {
  if (!currentTreeRoot_ || scancode == 0) return;

  KeyEvent event{static_cast<KeyboardKey>(key), static_cast<KeyAction>(action), mods};
  if (event.action == KeyAction::RELEASE || event.action == KeyAction::REPEAT) {
    // fmt::println("Key Event: key={}, scancode={}, action={}, mods={}", key, scancode, action, mods);
    currentTreeRoot_->handleKeyEvent(event);
  }
}

void UIManager::sendCharEvent(unsigned int codepoint) {
  // Logger::log(fmt::format("Char Event: codepoint={} (char='{}')", codepoint, static_cast<char>(codepoint)).c_str());
  if (currentTreeRoot_) {
    std::string utf8Char = std::string(1, static_cast<char>(codepoint));
    currentTreeRoot_->handleCharEvent(utf8Char);
  }
}

void UIManager::sendMouseEvent(double xpos, double ypos) {
  // fmt::println("Mouse Event: xpos={}, ypos={}", xpos, ypos);

  if (!currentTreeRoot_) return;
  std::shared_ptr<UIComponent> newHoveredComponent = nullptr;

  std::function<void(const std::shared_ptr<UIComponent> &)> findHovered =
      [&](const std::shared_ptr<UIComponent> &component) {
        if (component && component->getGobalBounds().contains(xpos, ypos)) {
          if (!component || !component->getGobalBounds().contains(xpos, ypos)) {
            return;
          }

          newHoveredComponent = component;
          for (const auto &child : component->children()) {
            findHovered(child);
          }
        }
      };

  findHovered(currentTreeRoot_);

  if (newHoveredComponent != currentHoveredComponent_) {
    if (currentHoveredComponent_) currentHoveredComponent_->onHoverExit();
    if (newHoveredComponent) newHoveredComponent->onHoverEnter();
    currentHoveredComponent_ = newHoveredComponent;
  }

  if (currentHoveredComponent_) {
    Offset offset{static_cast<float>(xpos), static_cast<float>(ypos)};
    currentHoveredComponent_->onHoverMove(offset);
  }
}

void UIManager::releaseAllFocus(std::optional<std::shared_ptr<UIComponent>> component) {
  if (!component.has_value() && !currentTreeRoot_) return;

  if (!component.has_value() && currentTreeRoot_) component = currentTreeRoot_;
  const auto &children = component->get()->children();

  if (children.empty() && component->get()->getChild()) {
    component->get()->getChild()->setFocus(false);
    releaseAllFocus(component->get()->getChild());
  }

  for (const auto &child : children) {
    child->setFocus(false);
    releaseAllFocus(child);
  }
}

void UIManager::requestFocus(UIComponent &component) {
  releaseAllFocus(std::nullopt);
  component.setFocus(true);
}

void UIManager::releaseFocus(UIComponent &component) { component.setFocus(false); }
