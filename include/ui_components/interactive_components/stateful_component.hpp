#pragma once

#include <memory>
#include "basic/ui_component.hpp"

class StatefulComponent : public UIComponent {
 public:
  StatefulComponent() : isDirty_(false), cachedBody_(nullptr) {}
  virtual ~StatefulComponent() = default;

  virtual std::shared_ptr<UIComponent> body() = 0;

 protected:
  void markDirty();

  std::shared_ptr<UIComponent> getChild();

  void layout(UISize size) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

  virtual bool processChildTaps(const UITapEvent &event) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  std::string toString(int indent = 0) const override;

  void handleCharEvent(char letter) noexcept override;

  void handleKeyEvent(KeyEvent &key) noexcept override;

 private:
  bool isDirty_;
  std::shared_ptr<UIComponent> cachedBody_;
  mutable std::vector<std::shared_ptr<UIComponent>> statefulChildren_;
};

inline bool StatefulComponent::processChildTaps(const UITapEvent &event) {
  const auto child = getChild();
  if (child) {
    UITapEvent localEvent = event;
    localEvent.x -= bounds_.x;
    localEvent.y -= bounds_.y;

    return child->processTap(localEvent);
  }
  return false;
}

inline void StatefulComponent::handleCharEvent(char letter) noexcept {
  if (cachedBody_) cachedBody_->handleCharEvent(letter);
}

inline void StatefulComponent::handleKeyEvent(KeyEvent &key) noexcept {
  if (cachedBody_) cachedBody_->handleKeyEvent(key);
}
