#pragma once

#include <fmt/base.h>
#include "rect.hpp"
#include "size.hpp"
#include "ui_alignment.hpp"
#include "ui_component.hpp"
#include "axis.hpp"

struct FlexParam {
  float spacing = 0.0f;
  Axis axis = Axis::VERTICAL;
  // MainAxisSize mainAxisSize = MainAxisSize::FIT;
  MainAxisAlignment mainAxisAlignment = MainAxisAlignment::START;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
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

  float getChildMainAxisSize(UIRect bound) const;
  float getChildCrossAxisSize(UIRect bound) const;

  UIRect setMainAxisSize(float contentSize, UIRect bound, UISize parentSize) const;
  UIRect setCrossAxisSize(float contentSize, UIRect bound) const;

  float getCrossAxisStartPosition(float avalibleSize, float childSize) const;
  float getMainAxisStartPosition(float availableSizeLeft) const;

  float getMainAxisSpacing(float availableSizeLeft) const noexcept;

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

inline float FlexBox::getChildMainAxisSize(UIRect bound) const {
  switch (param_.axis) {
    case Axis::HORIZONTAL:
      return bound.width;
    case Axis::VERTICAL:
      return bound.height;
  }
  return 0.0f;
};

inline float FlexBox::getChildCrossAxisSize(UIRect bound) const {
  switch (param_.axis) {
    case Axis::HORIZONTAL:
      return bound.height;
    case Axis::VERTICAL:
      return bound.width;
  }
  return 0.0f;
};

inline UIRect FlexBox::setMainAxisSize(float contentSize, UIRect bound, UISize parentSize) const {
  contentSize = std::fmax(0, contentSize);

  switch (param_.axis) {
    case Axis::HORIZONTAL:
      // if (param_.mainAxisSize == MainAxisSize::FILL) {
      //   return bound.copyWith({.width = parentSize.width});
      // }

      return bound.copyWith({.width = contentSize});
    case Axis::VERTICAL:
      // if (param_.mainAxisSize == MainAxisSize::FILL) {
      //   return bound.copyWith({.height = parentSize.height});
      // }

      return bound.copyWith({.height = contentSize});
  }
  return bound;
}

inline UIRect FlexBox::setCrossAxisSize(float contentSize, UIRect bound) const {
  contentSize = std::fmax(0, contentSize);

  switch (param_.axis) {
    case Axis::HORIZONTAL:
      return bound.copyWith({.height = contentSize});
    case Axis::VERTICAL:
      return bound.copyWith({.width = contentSize});
  }
  return bound;
}

inline float FlexBox::getCrossAxisStartPosition(float availableWidth, float childWidth) const {
  switch (param_.crossAxisAlignment) {
    case CrossAxisAlignment::START:
      return 0.0f;
    case CrossAxisAlignment::CENTER:
      return (availableWidth - childWidth) / 2;
    case CrossAxisAlignment::END:
      return availableWidth - childWidth;
  }

  return 0.0f;
}

inline float FlexBox::getMainAxisStartPosition(float availableSizeLeft) const {
  const auto childCount = param_.children.size();

  switch (param_.mainAxisAlignment) {
    case MainAxisAlignment::STRETCH:
    case MainAxisAlignment::START:
      return 0.0f;
    case MainAxisAlignment::CENTER:
      return availableSizeLeft / 2.0f;
    case MainAxisAlignment::END:
      return availableSizeLeft;
  }

  return 0.0f;
};

inline float FlexBox::getMainAxisSpacing(float availableSizeLeft) const noexcept {
  const auto spacing = param_.spacing;
  const auto childCount = param_.children.size();

  switch (param_.mainAxisAlignment) {
    case MainAxisAlignment::START:
    case MainAxisAlignment::CENTER:
    case MainAxisAlignment::END:
    case MainAxisAlignment::STRETCH:
      return spacing;
  }

  return spacing;
}
