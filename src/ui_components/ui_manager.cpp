#include "ui_manager.hpp"

#include <fmt/base.h>
#include <include/core/SkCanvas.h>

#include <memory>

#include "foundation/skia_font_helper.hpp"
#include "render/render_object.hpp"

UIManager &UIManager::instance() {
  static UIManager instance;
  return instance;
}

UIManager::UIManager() {
  fontMgr_ = CreatePlatformFontMgr();

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
  if (currentElementTreeRoot_) {
    const auto &ro = currentElementTreeRoot_->getRenderObject();
    bool handled = ro->processTap(event);
    if (handled) {
      fmt::println("Tap handled by UI at ({}, {})", event.x, event.y);
    } else {
      fmt::println("Tap not handled at ({}, {})", event.x, event.y);
    }
  }
}

void UIManager::setScrollCallback(const Offset &offset) {
  // if (currentTreeRoot_) {
  //   currentTreeRoot_->setCursorPosCallback(offset);
  // }
}

void UIManager::setTree(const UIElementPtr rootElement, SkCanvas *canvas, float w, float h, bool needsResize) {
  KeyPool::instance().reset();
  UILogger::instance().update();
  if (rootElement && rootElement->getRenderObject()) {
    const UIConstraints &constraints = UIConstraints::maxSize(w, h);
    auto ro = rootElement->getRenderObject();

    ro->performLayout(constraints);
    ro->paint(canvas);
  }

  currentElementTreeRoot_ = rootElement;
  previousElementTreeRoot_ = currentElementTreeRoot_;

  // rootUI->draw(skiaRenderer.getCanvas());
  //
  // currentTreeRoot_ = tree;
  // width_ = w;
  // height_ = h;
  // currentTreeRoot_->layout(UIConstraints{0, w, 0, h});
  // previousTreeRoot_ = currentTreeRoot_;
}

void UIManager::sendKeyEvent(int key, int scancode, int action, int mods) {
  if (!currentElementTreeRoot_ || scancode == 0) return;

  if (!currentElementTreeRoot_->getRenderObject()) return;
  const auto &ro = currentElementTreeRoot_->getRenderObject();

  KeyEvent event{static_cast<KeyboardKey>(key), static_cast<KeyAction>(action), mods};
  if (event.action == KeyAction::RELEASE || event.action == KeyAction::REPEAT) {
    // fmt::println("Key Event: key={}, scancode={}, action={}, mods={}", key, scancode, action, mods);
    ro->handleKeyEvent(event);
  }
}

void UIManager::sendCharEvent(unsigned int codepoint) {
  // Logger::log(fmt::format("Char Event: codepoint={} (char='{}')", codepoint, static_cast<char>(codepoint)).c_str());

  if (currentElementTreeRoot_) {
    if (!currentElementTreeRoot_->getRenderObject()) return;
    const auto &ro = currentElementTreeRoot_->getRenderObject();

    std::string utf8Char = std::string(1, static_cast<char>(codepoint));
    ro->handleCharEvent(utf8Char);
  }
}

void UIManager::sendMouseEvent(double xpos, double ypos) {
  // // fmt::println("Mouse Event: xpos={}, ypos={}", xpos, ypos);
  //
  // if (!currentTreeRoot_) return;
  // std::shared_ptr<UIComponent> newHoveredComponent = nullptr;
  //
  // std::function<void(const std::shared_ptr<UIComponent> &)> findHovered =
  //     [&](const std::shared_ptr<UIComponent> &component) {
  //       if (component && component->getGobalBounds().contains(xpos, ypos)) {
  //         if (!component || !component->getGobalBounds().contains(xpos, ypos)) {
  //           return;
  //         }
  //
  //         newHoveredComponent = component;
  //         for (const auto &child : component->children()) {
  //           findHovered(child);
  //         }
  //       }
  //     };
  //
  // findHovered(currentTreeRoot_);
  //
  // if (newHoveredComponent != currentHoveredComponent_) {
  //   if (currentHoveredComponent_) currentHoveredComponent_->onHoverExit();
  //   if (newHoveredComponent) newHoveredComponent->onHoverEnter();
  //   currentHoveredComponent_ = newHoveredComponent;
  // }
  //
  // if (currentHoveredComponent_) {
  //   Offset offset{static_cast<float>(xpos), static_cast<float>(ypos)};
  //   currentHoveredComponent_->onHoverMove(offset);
  // }
}

void UIManager::releaseAllFocus(std::optional<RenderObjectPtr> component) {
  if (!component.has_value() && !currentElementTreeRoot_) return;

  if (!component.has_value() && currentElementTreeRoot_) {
    component = currentElementTreeRoot_->getRenderObject();
  }
  const auto &children = component->get()->getChildren();

  if (children.empty() && component->get()->getChild()) {
    component->get()->getChild()->setFocus(false);
    releaseAllFocus(component->get()->getChild());
  }

  for (const auto &child : children) {
    child->setFocus(false);
    releaseAllFocus(child);
  }
}

void UIManager::requestFocus(const RenderObjectPtr &ro) {
  releaseAllFocus(std::nullopt);
  ro->setFocus(true);
}

void UIManager::releaseFocus(const RenderObjectPtr &ro) { ro->setFocus(false); }
