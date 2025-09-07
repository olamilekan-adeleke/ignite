#pragma once

#include <memory>
#include <vector>

#include "ui_alignment.hpp"
#include "ui_component.hpp"

struct ColumnParams {
  float spacing = 0.0f;
  MainAxisAlignment mainAxisAlignment = MainAxisAlignment::START;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  CrossAxisSize crossAxisSize = CrossAxisSize::FIT;
  MainAxisSize mainAxisSize = MainAxisSize::FIT;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class Column : public UIComponent {
 public:
  Column(const ColumnParams &params = {});

  void addChild(std::shared_ptr<UIComponent> child);

  void layout(UISize size) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

  bool wantsToFillMainAxis() const override;

  bool wantsToFillCrossAxis() const override;

 protected:
  virtual bool processChildTaps(const UITapEvent &event) override {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
      if ((*it)->processTap(event)) return true;
    }
    return false;
  }

 private:
  inline float getXPosition(CrossAxisAlignment axis, float width, float childWidth) {
    switch (axis) {
      case CrossAxisAlignment::START:
        return 0;
      case CrossAxisAlignment::CENTER:
        return (width - childWidth) / 2.0f;
      case CrossAxisAlignment::END:
        return width - childWidth;
    }
    return 0.0f;
  }

  inline float getMainAxisStartPosition(MainAxisAlignment axis,
                                        float availableSpace,
                                        float totalContentHeight,
                                        size_t childCount) {
    switch (axis) {
      case MainAxisAlignment::START:
        return 0;
      case MainAxisAlignment::CENTER:
        return availableSpace / 2.0f;
      case MainAxisAlignment::END:
        return availableSpace;
      case MainAxisAlignment::SPACE_BETWEEN:
        return 0;
      case MainAxisAlignment::SPACE_AROUND:
        return availableSpace / (2.0f * childCount);
      case MainAxisAlignment::SPACE_EVENLY:
        return availableSpace / (childCount + 1);
    }
    return 0;
  }

  inline float getSpaceBetweenChildren(MainAxisAlignment axis,
                                       float availableSpace,
                                       size_t childCount,
                                       float originalSpacing) {
    switch (axis) {
      case MainAxisAlignment::START:
      case MainAxisAlignment::CENTER:
      case MainAxisAlignment::END:
        return originalSpacing;
      case MainAxisAlignment::SPACE_BETWEEN:
        return childCount > 1 ? availableSpace / (childCount - 1) : 0;
      case MainAxisAlignment::SPACE_AROUND:
        return availableSpace / childCount;
      case MainAxisAlignment::SPACE_EVENLY:
        return availableSpace / (childCount + 1);
    }
    return originalSpacing;
  }

  float spacing_;
  CrossAxisAlignment crossAxisAlignment_;
  MainAxisAlignment mainAxisAlignment_;
  CrossAxisSize crossAxisSize_;
  MainAxisSize mainAxisSize_;
  std::vector<std::shared_ptr<UIComponent>> children_;
};
