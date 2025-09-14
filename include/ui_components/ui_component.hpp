#pragma once

#include <fmt/base.h>
#include <include/core/SkCanvas.h>

#include <memory>
#include <vector>
#include "basic/ui_render_object.hpp"
#include "basic/ui_tap_handler.hpp"
#include "rect.hpp"
#include "tap_event.hpp"
#include "key.hpp"

class UIComponent : public UIRenderObject, public UITapHandler {
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

 protected:
  virtual bool processChildTaps(const UITapEvent &event) { return false; }

  UIRect bounds_;
  UIKey key_;

  TapListener tapListener_;
};
