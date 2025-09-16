#include <fmt/base.h>
#include "rect.hpp"
#include "size.hpp"
#include "ui_component.hpp"
#include "axis.hpp"

struct FlexParam {
  float spacing = 0.0f;
  Axis flexAxis = Axis::VERTICAL;
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
  virtual bool processChildTaps(const UITapEvent& event) override {
    for (auto i = children_.rbegin(); i != children_.rend(); ++i) {
      if ((*i)->processTap(event)) return true;
    }
    return false;
  }

  float getChildMainAxisSize(UIRect bound);
  float getChildCrossAxisSize(UIRect bound);

  UIRect setMainAxisSize(float contentSize, UIRect bound);
  UIRect setCrossAxisSize(float contentSize, UIRect bound);

 private:
  FlexParam param_;
  std::vector<std::shared_ptr<UIComponent>> children_;
};

inline float FlexBox::getChildMainAxisSize(UIRect bound) {
  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      return bound.width;
    case Axis::VERTICAL:
      return bound.height;
  }
};

inline float FlexBox::getChildCrossAxisSize(UIRect bound) {
  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      return bound.height;
    case Axis::VERTICAL:
      return bound.width;
  }
};

inline UIRect FlexBox::setMainAxisSize(float contentSize, UIRect bound) {
  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      return bound.copyWith({.width = contentSize});
    case Axis::VERTICAL:
      return bound.copyWith({.height = contentSize});
  }
}

inline UIRect FlexBox::setCrossAxisSize(float contentSize, UIRect bound) {
  switch (param_.flexAxis) {
    case Axis::HORIZONTAL:
      return bound.copyWith({.height = contentSize});
    case Axis::VERTICAL:
      return bound.copyWith({.width = contentSize});
  }
}
