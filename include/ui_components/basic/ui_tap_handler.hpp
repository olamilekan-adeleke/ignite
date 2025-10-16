#pragma once

#include <functional>

#include "offset.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "tap_event.hpp"

using TapListener = std::function<void(const UITapEvent& event)>;
using VoidCallBack = std::function<void()>;

template <typename T>
using ValueChangedListener = std::function<void(const T& newValue)>;

class UITapHandler {
 public:
  UITapHandler() : tappable_(true) {}
  virtual ~UITapHandler() = default;

  // Override this method to customize hit detection logic
  virtual bool hitTest(const UITapEvent& event, const UIRect& bounds) const {
    return bounds.contains(event.x, event.y);
  }

  virtual bool hitTest(const Offset& offset, const UIRect& bounds) const { return bounds.contains(offset.x, offset.y); }

  bool onTap(const UITapEvent& event, const UIRect& bounds) {
    if (!tapListener_.has_value()) return false;

    if (hitTest(event, bounds)) {
      UITapEvent localEvent = event;
      localEvent.x = event.x - bounds.x;
      localEvent.y = event.y - bounds.y;
      (*tapListener_)(localEvent);
      return true;
    }
    return false;
  }

  void setTapListener(TapListener listener) {
    if (!listener) return;
    tapListener_ = std::move(listener);
  }

  void clearTapListener() { tapListener_.reset(); }

  void setTapEnabled(bool enabled) { tappable_ = enabled; }

  bool isTapEnabled() const { return tappable_; }

  bool hasTapListener() const { return tapListener_.has_value(); }

 protected:
  std::optional<TapListener> tapListener_;
  bool tappable_;
};

inline UITapEvent translateEvent(const UITapEvent& event, const Vec2& offset) {
  return {event.x - offset.x, event.y - offset.y};
}

inline UITapEvent translateToLocal(const UITapEvent& event, const Offset& globalOffset) noexcept {
  UITapEvent localEvent = event;
  localEvent.x -= globalOffset.x;
  localEvent.y -= globalOffset.y;
  return localEvent;
}
