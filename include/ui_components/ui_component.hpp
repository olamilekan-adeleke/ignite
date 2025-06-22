#pragma once

#include <functional>
#include <fmt/base.h>
#include <include/core/SkCanvas.h>

#include <memory>
#include <vector>

#include "rect.hpp"
#include "tap_event.hpp"
#include "ui_key.hpp"

using TapListener = std::function<void(const UITapEvent &event)>;

class UIComponent {
 public:
  UIComponent() : key_(UIKey()), tappable_(true) {}

  virtual ~UIComponent() = default;

  virtual void layout(float parentWidth, float parentHeight) = 0;
  virtual void draw(SkCanvas *canvas);

  UIRect getBounds() const { return bounds_; }

  void setPosition(float x, float y) {
    bounds_.x = x;
    bounds_.y = y;
  }

  void setSize(float w, float h) {
    bounds_.width = w;
    bounds_.height = h;
  }

  void setKey(UIKey key) { key_ = key; }
  UIKey key() const { return key_; }

  virtual const std::vector<std::shared_ptr<UIComponent>> &children() const {
    static const std::vector<std::shared_ptr<UIComponent>> empty_children;
    return empty_children;
  }

  void setTapListener(TapListener listener) { tapListener_ = std::move(listener); }

  void setTapEnabled(bool enabled) { tappable_ = enabled; }

  bool isTapEnabled() const { return tappable_; }

  virtual bool processTap(const UITapEvent &event) {
    if (event.consumed) return false;
    if (processChildTaps(event)) return true;

    return onTap(event);
  }

  virtual bool tapWithBounds(float x, float y) const {
    // so x have to be withing the view how x area, which is like x-pos plus the width (that is the horizontal area)
    // same for y too
    // TODO: come back to see if i can improve this
    return x >= bounds_.x && x < bounds_.x + bounds_.width && y >= bounds_.y && y < bounds_.y + bounds_.height;
  }

  virtual bool onTap(const UITapEvent &event) const {
    const bool inBounds = tapWithBounds(event.x, event.y);
    if (tappable_ && tapListener_ && inBounds) {
      // translate the (x, y) to the local bounds, the child width can do more complex gesture detection
      UITapEvent localEvent = event;
      localEvent.x = event.x - bounds_.x;
      localEvent.y = event.y - bounds_.y;
      tapListener_(localEvent);
      return true;
    }
    return false;
  }

 protected:
  virtual bool processChildTaps(const UITapEvent &event) { return false; }

  UIRect bounds_;
  UIKey key_;
  TapListener tapListener_;
  bool tappable_;

  static SkPaint debug_border_paint_;
  static bool debug_paint_initialized_;

  void initializeDebugPaint();
};
