#pragma once

#include <memory>
#include <vector>

#include "component/state_base_component.hpp"
#include "render/render_object.hpp"

struct LayoutBoxParam {
  Axis axis = Axis::VERTICAL;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  MainAxisSize sizing = MainAxisSize::FIT;
  float childGap = 0;
};

class LayoutRenderObject : public RenderObject {
 public:
  LayoutRenderObject(const LayoutBoxParam &param) : params_(param) {}

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas *canvas) noexcept override;

  void update(const LayoutBoxParam &param) { params_ = std::move(param); }

 protected:
  bool wantsToFillMainAxis() const override { return false; }
  bool wantsToFillCrossAxis() const override { return false; }

  float distributeFlexSpace(float availableSize, uint numChildren, RenderObjectPtr child) const noexcept;

  float getMainAxisSize(const UISizing &size) const noexcept;
  float getCrossAxisSize(const UISizing &size) const noexcept;

  float getCrossAxisPosition(const UISizing &size) const noexcept;

  bool processChildTaps(const UITapEvent &event) override {
    const auto bounds_ = getBounds();
    if (!bounds_.contains(event.x, event.y)) return false;

    UITapEvent localEvent = event;
    localEvent.x = event.x - bounds_.x;
    localEvent.y = event.y - bounds_.y;
    for (auto &child : children_) {
      if (child->processTap(localEvent)) return true;
    }
    return false;
  }

 private:
  LayoutBoxParam params_;
};

class LayoutBox : public MultiChildStatelessComponent {
 public:
  LayoutBox(const LayoutBoxParam &param = {}, const UIKey &key = {})
      : params_(std::move(param)), MultiChildStatelessComponent(key) {};

  // void layout(UIConstraints size) override;
  // void draw(SkCanvas *canvas) override;

  // const std::vector<std::shared_ptr<UIComponent>> &children() const override;

  std::vector<ComponentPtr> build() override { return children_; }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<LayoutRenderObject>(params_); }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<LayoutRenderObject>(ro)->update(params_);
  }

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const noexcept override {
    MultiChildStatelessComponent::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "axis: " << axisToString(params_.axis) << "\n";
    os << pad << "children: " << children_.size() << "\n";
    os << pad << "childGap: " << params_.childGap << "\n";
  }

  // bool wantsToFillMainAxis() const override { return false; }
  // bool wantsToFillCrossAxis() const override { return false; }
  //
  // float distributeFlexSpace(float availableSize, uint numChildren, std::shared_ptr<UIComponent> child) const
  // noexcept;
  //
  // float getMainAxisSize(const UISizing &size) const noexcept;
  // float getCrossAxisSize(const UISizing &size) const noexcept;
  //
  // float getCrossAxisPosition(const UISizing &size) const noexcept;

 private:
  LayoutBoxParam params_;
};
