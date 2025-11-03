#pragma once

#include <memory>

#include "component/state_base_component.hpp"
#include "foundation/utils/key.hpp"
#include "render/render_object.hpp"
#include "transformer/transfrom_matrix.hpp"

struct TransformParam {
  IgniteTransform::Matrix3 transform;
};

class TransformRenderObject : public RenderObject {
 public:
  TransformRenderObject(const TransformParam& param) : param_(param) {}

  void performLayout(UIConstraints constraints) noexcept override {
    float width = constraints.maxWidth;
    float height = constraints.maxHeight;

    const auto& child = getChild(0);
    if (child) {
      child->performLayout(UIConstraints::maxSize(width, height));
      const auto& childSize = child->getSize();
      setSize(childSize.width, childSize.height);
    }
  }

  void paint(SkCanvas* canvas) noexcept override {
    const auto& child = getChild(0);
    if (!child) return;

    SkAutoCanvasRestore save(canvas, true);

    const UIRect& bounds = getBounds();
    canvas->translate(bounds.x, bounds.y);

    // Get child size for center calculation
    const auto& childSize = child->getSize();
    float centerX = childSize.width / 2;
    float centerY = childSize.height / 2;

    // Translate to center -> apply transform -> translate back
    canvas->translate(centerX, centerY);
    SkMatrix skMatrix = param_.transform.toSkMatrix();
    canvas->concat(skMatrix);
    canvas->translate(-centerX, -centerY);

    child->paint(canvas);
  }

  void updateParam(const TransformParam& param) { param_ = param; }

 private:
  TransformParam param_;
};

class TransformComponent : public StatelessComponent {
 public:
  TransformComponent(const TransformParam& param, const UIKey& key = {}) : param_(param), StatelessComponent(key) {}

  ComponentPtr build() override { return getChild(); }

  RenderObjectPtr createRenderObject() const noexcept override {
    return std::make_shared<TransformRenderObject>(param_);
  }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<TransformRenderObject>(ro)->updateParam(param_);
  }

 private:
  TransformParam param_;
};
