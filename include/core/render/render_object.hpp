#pragma once

#include <include/core/SkCanvas.h>

#include <memory>

#include "foundation/foundation.hpp"

class RenderObject;
using RenderObjectPtr = std::shared_ptr<RenderObject>;

class RenderObject : public std::enable_shared_from_this<RenderObject> {
 public:
  virtual ~RenderObject() = default;

  virtual void performLayout(UIConstraints constraints) noexcept {}
  virtual void paint(SkCanvas* canvas) noexcept;

  UIRect getBounds() const noexcept { return bounds_; }
  void setPosition(float x, float y) noexcept { bounds_.x = x, bounds_.y = y; }

  void setSize(float w, float h) noexcept { bounds_.width = w, bounds_.height = h; }
  void setSize(const UISize& size) noexcept { bounds_.width = size.width, bounds_.height = size.height; }
  UISizing getSize() const noexcept { return UISizing::Fixed(bounds_.width, bounds_.height); }

  RenderObjectPtr getParent() const noexcept { return parent; }
  void setParent(RenderObjectPtr parent) noexcept { this->parent = parent; }

  const std::vector<RenderObjectPtr>& getChildren() const noexcept { return children_; }
  const RenderObjectPtr& getChild(int index) const noexcept {
    if (index < 0 || index >= children_.size()) {
      static RenderObjectPtr null_ptr = nullptr;
      return null_ptr;
    }

    return children_[index];
  }
  void setChildren(const std::vector<RenderObjectPtr>& children) noexcept { this->children_ = children; }
  void clearChildren() noexcept { children_.clear(); }

  void addChild(RenderObjectPtr child) noexcept {
    if (!child) return;

    children_.push_back(child);
    child->setParent(shared_from_this());
  }

  void removeChild(RenderObjectPtr child) noexcept {
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) children_.erase(it);
    child->setParent(nullptr);
  }

 protected:
  std::vector<RenderObjectPtr> children_;

 private:
  RenderObjectPtr parent;

  UIRect bounds_{0, 0, 0, 0};

  static SkPaint debug_border_paint_;
  static bool debug_paint_initialized_;

  void initializeDebugPaint();
};
