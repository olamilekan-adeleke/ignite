#pragma once

#include <sstream>
#include <fmt/base.h>
#include "basic/ui_component.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "ui_alignment.hpp"
#include "axis.hpp"

struct FlexParam {
  float spacing = 0.0f;
  Axis axis = Axis::VERTICAL;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class FlexBox : public UIComponent {
 public:
  FlexBox(const FlexParam& param = {});

  void addChild(std::shared_ptr<UIComponent> child);

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  const std::vector<std::shared_ptr<UIComponent>>& children() const override;

  bool wantsToFillMainAxis() const override;
  bool wantsToFillCrossAxis() const override;

 protected:
  virtual bool processChildTaps(const UITapEvent& event) override;

  void debugFillProperties(std::ostringstream& os, int indent) const override;

  float getChildMainAxisSize(UIRect bound) const;
  float getChildCrossAxisSize(UIRect bound) const;

  UIRect setMainAxisSize(float contentSize, UIRect bound, UISize parentSize) const;
  UIRect setCrossAxisSize(float contentSize, UIRect bound) const;

  float getCrossAxisStartPosition(float avalibleSize, float childSize) const;
  float getMainAxisStartPosition(float availableSizeLeft) const;

  float getMainAxisSpacing(float availableSizeLeft) const noexcept;

  void handleCharEvent(char letter) noexcept override;

 private:
  FlexParam param_;
};

inline void FlexBox::handleCharEvent(char letter) noexcept {
  for (auto& child : param_.children) {
    child->handleCharEvent(letter);
  }
}

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

inline float FlexBox::getMainAxisStartPosition(float availableSizeLeft) const { return 0.0f; };

inline float FlexBox::getMainAxisSpacing(float availableSizeLeft) const noexcept { return param_.spacing; }

inline void FlexBox::debugFillProperties(std::ostringstream& os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "spacing: " << param_.spacing << "\n";
  os << pad << "axis: " << axisToString(param_.axis) << "\n";
  os << pad << "children: " << param_.children.size() << "\n";
  os << pad << "crossAxisAlignment: " << param_.crossAxisAlignment << "\n";
}
