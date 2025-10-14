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

  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

  virtual bool processChildTaps(const UITapEvent &event) override;

  std::string toString(int indent = 0) const override;

 private:
  bool isDirty_;
  std::shared_ptr<UIComponent> cachedBody_;
  mutable std::vector<std::shared_ptr<UIComponent>> childrenCache_;
};

inline const std::vector<std::shared_ptr<UIComponent>> &StatefulComponent::children() const {
  if (cachedBody_) {
    if (childrenCache_.empty() || childrenCache_[0] != cachedBody_) {
      childrenCache_.clear();
      childrenCache_.push_back(cachedBody_);
    }
    return childrenCache_;
  }
  return UIComponent::children();
}

inline bool StatefulComponent::processChildTaps(const UITapEvent &event) {
  const auto child = getChild();
  if (child) {
    UITapEvent localEvent = event;
    // localEvent.x -= bounds_.x;
    // localEvent.y -= bounds_.y;

    return child->processTap(localEvent);
  }
  return false;
}
