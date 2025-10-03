#pragma once

#include <fmt/base.h>
#include <include/core/SkCanvas.h>

#include <memory>
#include <vector>

#include "basic/ui_hover_handler.hpp"
#include "basic/ui_render_object.hpp"
#include "basic/ui_tap_handler.hpp"
#include "basic/ui_text_input_handler.hpp"
#include "offset.hpp"
#include "tap_event.hpp"

enum class UIMarkDirtyType : uint8_t { LAYOUT, DRAW };

enum class UIMarkDirtyCaller : uint8_t { PARENT_TO_ChILD, NONE };

class UIComponent : public UIRenderObject, public UITapHandler, public UITextInputHandler, public UIHoverHandler {
 public:
  UIComponent() {}
  virtual ~UIComponent() = default;

  virtual const std::vector<std::shared_ptr<UIComponent>> &children() const {
    static const std::vector<std::shared_ptr<UIComponent>> empty_children;
    return empty_children;
  }

  virtual bool processTap(const UITapEvent &event) {
    if (processChildTaps(event)) return true;
    return onTap(event, bounds_);
  }

  std::string toString(int indent = 0) const override;

  virtual void markHasDirty(const UIMarkDirtyType &type, const UIMarkDirtyCaller &caller) noexcept {};

  void handleCharEvent(std::string letter) noexcept override;
  void handleKeyEvent(KeyEvent &key) noexcept override;

  void setChild(std::shared_ptr<UIComponent> child) { child_ = child; }

  std::shared_ptr<UIComponent> getChild() const { return child_; }

  const Offset &getGlobalOffset() const { return globalOffset_; }
  void setGlobalOffset(const Offset &offset) { globalOffset_ = offset; }

  UIRect getGobalBounds() const {
    return UIRect{
        .x = globalOffset_.x,
        .y = globalOffset_.y,
        .width = bounds_.width,
        .height = bounds_.height,
    };
  }

  virtual void updateGlobalOffset(const Offset &parentOffset) {
    globalOffset_.x = parentOffset.x + bounds_.x;
    globalOffset_.y = parentOffset.y + bounds_.y;

    for (const auto &child : children()) {
      child->updateGlobalOffset(globalOffset_);
    }
  }

  void onHoverMove(Offset &mousePosition) noexcept override {
    UIRect boundsUIRect{getGobalBounds()};

    const auto children = this->children();
    if (!children.empty()) {
      for (auto &child : children) {
        child->onHoverMove(mousePosition);
      }
    } else if (children.empty() && child_) {
      child_->onHoverMove(mousePosition);
    } else {
      if (!this->shouldHandleHover()) return;

      if (hitTest(mousePosition, boundsUIRect)) {
        onHoverEnter();
        fmt::println("shouldHandleHover is {} for {} | {} mouse: {}x{}",
                     (*this).shouldHandleHover(),
                     typeid(*this).name(),
                     boundsUIRect.toStrin(),
                     mousePosition.x,
                     mousePosition.y);
        // fmt::println("onHoverEnter");
      } else {
        onHoverExit();
      }
    }
  }

  void setCursorPosCallback(Offset offset) noexcept override;

 protected:
  virtual bool processChildTaps(const UITapEvent &event) { return false; }

  // NOTE: this was added because i don't know why the UI layout break if View override children()
  // i have try debug it but i do not want to exhaust energy to fix it. This will do the job for now
  std::shared_ptr<UIComponent> child_;

 private:
  Offset globalOffset_{0.0f, 0.0f};
};

inline void UIComponent::handleCharEvent(std::string letter) noexcept {
  // if (!hasFocus()) return;

  const auto children = this->children();
  for (auto &child : children) {
    child->handleCharEvent(letter);
  }

  if (children.empty() && child_) child_->handleCharEvent(letter);
}

inline void UIComponent::handleKeyEvent(KeyEvent &key) noexcept {
  // if (!hasFocus()) return;

  const auto children = this->children();
  for (auto &child : children) {
    child->handleKeyEvent(key);
  }

  if (children.empty() && child_) child_->handleKeyEvent(key);
}

inline void UIComponent::setCursorPosCallback(Offset offset) noexcept {
  const auto children = this->children();

  if (!children.empty()) {
    for (auto &child : children) {
      child->setCursorPosCallback(offset);
    }
  }

  if (children.empty() && child_) child_->setCursorPosCallback(offset);
}
