#pragma once

#include <functional>
#include <fmt/base.h>
#include <include/core/SkCanvas.h>

#include <memory>
#include <vector>
#include "basic/ui_render_object.hpp"
#include "rect.hpp"
#include "tap_event.hpp"
#include "key.hpp"

using TapListener = std::function<void(const UITapEvent &event)>;

class UIComponent : public UIRenderObject {
 public:
  UIComponent() {}
  virtual ~UIComponent() = default;

  virtual const std::vector<std::shared_ptr<UIComponent>> &children() const {
    static const std::vector<std::shared_ptr<UIComponent>> empty_children;
    return empty_children;
  }

  void setTapListener(TapListener listener) { tapListener_ = std::move(listener); }

  virtual bool processTap(const UITapEvent &event) {
    // if (event.consumed) return false;
    if (processChildTaps(event)) return true;
    return onTap(event);
  }

  virtual bool tapWithBounds(float x, float y) const { return bounds_.contains(x, y); }

  virtual bool onTap(const UITapEvent &event) const {
    const bool inBounds = tapWithBounds(event.x, event.y);
    if (tapListener_ && inBounds) {
      UITapEvent localEvent = event;
      localEvent.x = event.x - bounds_.x;
      localEvent.y = event.y - bounds_.y;
      tapListener_(localEvent);
      return true;
    }
    return false;
  }

  std::string toString(int indent = 0) const override;

 protected:
  virtual bool processChildTaps(const UITapEvent &event) { return false; }

  UIRect bounds_;
  UIKey key_;

  TapListener tapListener_;
};
