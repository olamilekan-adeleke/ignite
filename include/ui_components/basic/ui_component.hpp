#pragma once

#include <fmt/base.h>
#include <include/core/SkCanvas.h>

#include <memory>
#include <vector>
#include "basic/ui_render_object.hpp"
#include "basic/ui_tap_handler.hpp"
#include "tap_event.hpp"

enum class UIMarkDirtyType : uint8_t { LAYOUT, DRAW };

enum class UIMarkDirtyCaller : uint8_t { PARENT_TO_ChILD, CHILD_TO_PARENT, NONE };

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

  void setParent(std::shared_ptr<UIComponent> parent) noexcept;

  std::string toString(int indent = 0) const override;

  virtual void markHasDirty(const UIMarkDirtyType &type, const UIMarkDirtyCaller &caller) noexcept {};

 protected:
  std::weak_ptr<UIComponent> parent_;

  virtual bool processChildTaps(const UITapEvent &event) { return false; }
};
