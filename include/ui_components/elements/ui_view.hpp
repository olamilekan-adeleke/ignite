#pragma once

#include <fmt/base.h>

#include <memory>
#include <sstream>

#include "component/state_base_component.hpp"
#include "foundation/foundation.hpp"

struct ViewParams {
  UIEdgeInsets insets = {0.0f, 0.0f, 0.0f, 0.0f};
  Color backgroundColor = Color::Clear();
  float borderRadius = 0.0f;
  bool antiAlias = true;
  bool tappable = true;
  TapListener onTap = nullptr;
};

class ViewRenderObject : public RenderObject {
 public:
  ViewRenderObject(const ViewParams &params) : params_(params) { setTapListener(params_.onTap); }

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas *canvas) noexcept override;

  bool processChildTaps(const UITapEvent &event) override {
    fmt::println("ViewRenderObject::processChildTaps {}", params_.onTap ? "true" : "false");

    const auto bounds_ = getBounds();
    const auto child = getChild();
    if (child) {
      UITapEvent localEvent = event;
      localEvent.x -= bounds_.x;
      localEvent.y -= bounds_.y;
      return child->processTap(localEvent);
    }

    if (params_.onTap) return RenderObject::onTap(event, bounds_);
    return false;
  }

  void updateParams(const ViewParams &params) noexcept { params_ = std::move(params); }

 private:
  ViewParams params_;
};

class UIView : public StatelessComponent {
 public:
  UIView(const ViewParams &params = {}, const UIKey key = {}) : params_(params), StatelessComponent(key) {}
  UIView(ComponentPtr child, const ViewParams &params = {}, const UIKey key = {})
      : params_(params), StatelessComponent(key) {
    addChild(child);
  }

  UIView(const UIView &) = delete;
  UIView &operator=(const UIView &) = delete;

  ComponentPtr build() override { return getChild(); }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<ViewRenderObject>(params_); }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    auto view = std::dynamic_pointer_cast<ViewRenderObject>(ro);
    if (view) view->updateParams(params_);
  }

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const noexcept override {
    StatelessComponent::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "insets: " << params_.insets.toString() << "\n";
    os << pad << "backgroundColor: " << params_.backgroundColor.toString() << "\n";
    os << pad << "borderRadius: " << params_.borderRadius << "\n";
    os << pad << "antiAlias: " << (params_.antiAlias ? "true" : "false") << "\n";
    os << pad << "tappable: " << (params_.tappable ? "true" : "false") << "\n";
  }

 private:
  ViewParams params_;
};
