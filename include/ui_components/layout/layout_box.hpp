#pragma once

#include <sstream>
#include <vector>

#include "basic/ui_component.hpp"
#include "size.hpp"
#include "ui_alignment.hpp"

struct LayoutBoxParam {
  Axis axis = Axis::VERTICAL;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  MainAxisSize sizing = MainAxisSize::FIT;
  float childGap = 0;
  std::vector<std ::shared_ptr<UIComponent>> children;
};

class LayoutBox : public UIComponent {
 public:
  LayoutBox(const LayoutBoxParam &param = {}) : params_(std::move(param)) {};

  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const override;

  bool processChildTaps(const UITapEvent &event) override {
    if (!bounds_.contains(event.x, event.y)) return false;

    UITapEvent localEvent = event;
    localEvent.x = event.x - bounds_.x;
    localEvent.y = event.y - bounds_.y;
    for (auto &child : params_.children) {
      if (child->processTap(localEvent)) return true;
    }
    return false;
  }

  bool wantsToFillMainAxis() const override { return false; }
  bool wantsToFillCrossAxis() const override { return false; }

  float distributeFlexSpace(float availableSize, uint numChildren, std::shared_ptr<UIComponent> child) const noexcept;

  float getMainAxisSize(const UISizing &size) const noexcept;
  float getCrossAxisSize(const UISizing &size) const noexcept;

  float getCrossAxisPosition(const UISizing &size) const noexcept;

 private:
  LayoutBoxParam params_;
};

inline void LayoutBox::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "axis: " << axisToString(params_.axis) << "\n";
  os << pad << "children: " << params_.children.size() << "\n";
  os << pad << "childGap: " << params_.childGap << "\n";
}
