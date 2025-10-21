#pragma once
#include <fmt/base.h>

#include <memory>

#include "foundation/foundation.hpp"
#include "render/render_object.hpp"

class Component;
class UIElement;
class StateBase;

using ComponentPtr = std::shared_ptr<Component>;
using UIElementPtr = std::shared_ptr<UIElement>;

class Component : public std::enable_shared_from_this<Component> {
 public:
  Component(const UIKey& key = AUTO_KEY) : key_(std::move(key)) {}
  virtual ~Component() = default;

  UIKey key() const { return key_; }
  UIKey key() { return key_; }

  void setKey(UIKey key) { key_ = std::move(key); }

  virtual UIElementPtr createElement() = 0;

  virtual RenderObjectPtr createRenderObject() const noexcept { return nullptr; }
  virtual void updateRenderObject(RenderObjectPtr object) noexcept {}

  static bool canUpdate(const ComponentPtr& oldComp, const ComponentPtr& newComp) noexcept {
    if (!oldComp || !newComp) return false;

    const bool sameType = typeid(*oldComp) == typeid(*newComp);
    return sameType && oldComp->key() == newComp->key();
  };

  virtual bool wantsToFillMainAxis() const noexcept { return false; }
  virtual bool wantsToFillCrossAxis() const noexcept { return false; }
  virtual bool wantsToFill() const noexcept { return wantsToFillMainAxis() && wantsToFillCrossAxis(); }

 private:
  UIKey key_;
};

class UIElement : public std::enable_shared_from_this<UIElement> {
 public:
  UIElement(ComponentPtr w) : widget_(std::move(w)) {}
  virtual ~UIElement() = default;

  RenderObjectPtr getRenderObject() const noexcept { return renderObject_; }
  ComponentPtr getComponont() const noexcept { return widget_; }

  void setElementChildren(const std::vector<UIElementPtr>& children) { this->children = children; }

  virtual void performRebuild() noexcept {}

  virtual void mount(UIElementPtr element) noexcept {
    parent_ = element;
    renderObject_ = widget_->createRenderObject();
    _attachRenderObject();
  }

  virtual void unmount() {
    for (auto& child : children) child->unmount();
    children.clear();
    _detachRenderObject();
  }

  virtual void update(ComponentPtr newComp) noexcept {
    widget_ = std::move(newComp);
    if (renderObject_) widget_->updateRenderObject(renderObject_);
  }

 protected:
  std::shared_ptr<StateBase> state;

  RenderObjectPtr _findAncestorRenderObject() const noexcept {
    for (UIElement* e = parent_.get(); e; e = e->parent_.get()) {
      if (auto renderer = e->getRenderObject()) return renderer;
    }
    return nullptr;
  }

  void _attachRenderObject() const noexcept {
    if (!renderObject_) return;
    if (auto renderer = _findAncestorRenderObject()) renderer->addChild(renderObject_);
  }

  void _detachRenderObject() const noexcept {
    if (!renderObject_) return;
    if (auto renderer = _findAncestorRenderObject()) renderer->removeChild(renderObject_);
  }

  std::vector<UIElementPtr> updateChildren(const std::vector<ComponentPtr>& newComponents) {
    std::vector<UIElementPtr> newChildren;
    newChildren.reserve(newComponents.size());

    size_t oldSize = children.size();
    size_t newSize = newComponents.size();

    fmt::println("Starting updateChildren: {} -> {}", oldSize, newSize);
    for (size_t i = 0; i < newSize; ++i) {
      ComponentPtr newComponent = newComponents[i];
      UIElementPtr oldChild = (i < oldSize) ? children[i] : nullptr;
      UIElementPtr newChild = nullptr;

      if (oldSize && Component::canUpdate(oldChild->getComponont(), newComponent)) {
        fmt::println("  └─ canUpdate: {} -> {}", oldChild->getComponont()->key().value(), newComponent->key().value());
        oldChild->update(newComponent);
        newChild = oldChild;
      } else {
        fmt::println("  └─ cannotUpdate: {} -> {}", newComponent->key().value(), typeid(*newComponent).name());
        if (oldChild) oldChild->unmount();
        newChild = newComponent->createElement();
        newChild->mount(shared_from_this());
      }

      newChildren.push_back(newChild);
    }

    // discard any spare remaining
    for (size_t i = newSize; i < oldSize; ++i) {
      fmt::println("  └─ unmounting: {}", children[i]->getComponont()->key().value());
      children[i]->unmount();
    }
    printf("Finished updateChildren\n");
    return newChildren;
  }

 private:
  UIElementPtr parent_;
  ComponentPtr widget_;
  RenderObjectPtr renderObject_;

  std::vector<UIElementPtr> children;
};
