#include "layout/h_flex_box.hpp"

#include "layout/layout_box.hpp"

LayoutBox HFlexBox::create(const HFlexParam& param) {
  LayoutBoxParam flexParam{
      .axis = Axis::HORIZONTAL,
      .crossAxisAlignment = param.crossAxisAlignment,
      .sizing = param.sizing,
      .childGap = param.childGap,
      .children = std::move(param.children),
  };
  return LayoutBox(flexParam);
}
