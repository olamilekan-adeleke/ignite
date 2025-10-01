#pragma once

#include <fmt/base.h>
#include <include/core/SkCanvas.h>

#include <memory>
#include <vector>

#include "basic/ui_render_object.hpp"
#include "basic/ui_tap_handler.hpp"
#include "basic/ui_text_input_handler.hpp"
#include "tap_event.hpp"

enum class UIMarkDirtyType : uint8_t { LAYOUT, DRAW };

enum class UIMarkDirtyCaller : uint8_t { PARENT_TO_ChILD, NONE };

class UIComponent : public UIRenderObject, public UITapHandler, public UITextInputHandler {
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

 protected:
  virtual bool processChildTaps(const UITapEvent &event) { return false; }

  // NOTE: this was added because i don't know why the UI layout break if View override children()
  // i have try debug it but i do not want to exhaust energy to fix it. This will do the job for now
  std::shared_ptr<UIComponent> child_;
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
