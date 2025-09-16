#pragma once

#include "rect.hpp"
#include "size.hpp"
#include "ui_alignment.hpp"
#include "ui_component.hpp"
#include "axis.hpp"

struct FlexParam {
  float spacing = 0.0f;
  Axis flexAxis = Axis::VERTICAL;
  MainAxisSize mainAxisSize = MainAxisSize::FIT;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class FlexBox : public UIComponent {
 public:
  FlexBox(const FlexParam& param = {});

  void addChild(std::shared_ptr<UIComponent> child);

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

  const std::vector<std::shared_ptr<UIComponent>>& children() const override;

  bool wantsToFillMainAxis() const override;
  bool wantsToFillCrossAxis() const override;

 protected:
  virtual bool processChildTaps(const UITapEvent& event) override;

  float getChildMainAxisSize(UIRect bound);
  float getChildCrossAxisSize(UIRect bound);

  UIRect setMainAxisSize(float contentSize, UIRect bound, UISize parentSize);
  UIRect setCrossAxisSize(float contentSize, UIRect bound);

 private:
  FlexParam param_;
};

inline bool FlexBox::processChildTaps(const UITapEvent& event) {
  if (!bounds_.contains(event.x, event.y)) return false;

  // Transform to local coordinates
  UITapEvent localEvent = event;
  localEvent.x = event.x - bounds_.x;
  localEvent.y = event.y - bounds_.y;

  for (auto& child : param_.children) {
    if (child->processTap(localEvent)) {
      return true;
    }
  }
  return false;
}

inline float FlexBox::getChildMainAxisSize(UIRect bound) {
  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      return bound.width;
    case Axis::VERTICAL:
      return bound.height;
  }
  return 0.0f;
};

inline float FlexBox::getChildCrossAxisSize(UIRect bound) {
  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      return bound.height;
    case Axis::VERTICAL:
      return bound.width;
  }
  return 0.0f;
};

inline UIRect FlexBox::setMainAxisSize(float contentSize, UIRect bound, UISize parentSize) {
  contentSize = std::fmax(0, contentSize);

  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      if (param_.mainAxisSize == MainAxisSize::FILL) {
        return bound.copyWith({.width = parentSize.width});
      }

      return bound.copyWith({.width = contentSize});
    case Axis::VERTICAL:
      if (param_.mainAxisSize == MainAxisSize::FILL) {
        return bound.copyWith({.height = parentSize.height});
      }

      return bound.copyWith({.height = contentSize});
  }
  return bound;
}

inline UIRect FlexBox::setCrossAxisSize(float contentSize, UIRect bound) {
  contentSize = std::fmax(0, contentSize);

  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      return bound.copyWith({.height = contentSize});
    case Axis::VERTICAL:
      return bound.copyWith({.width = contentSize});
  }
  return bound;
}
