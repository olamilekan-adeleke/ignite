#include <functional>
#include "rect.hpp"
#include "tap_event.hpp"

using TapListener = std::function<void(const UITapEvent& event)>;

class UITapHandler {
 public:
  UITapHandler() : tappable_(true) {}
  virtual ~UITapHandler() = default;

  // Override this method to customize hit detection logic
  virtual bool hitTest(const UITapEvent& event, const UIRect& bounds) const {
    return bounds.contains(event.x, event.y);
  }

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

  void setTapListener(TapListener listener) { tapListener_ = std::move(listener); }

  void clearTapListener() { tapListener_.reset(); }

  void setTapEnabled(bool enabled) { tappable_ = enabled; }

  bool isTapEnabled() const { return tappable_; }

  bool hasTapListener() const { return tapListener_.has_value(); }

 protected:
  std::optional<TapListener> tapListener_;
  bool tappable_;
};
