#include "layout/v_flex_box.hpp"

#include "layout/layout_box.hpp"

LayoutBox VFlexBox::create(const VFlexParam& param) {
  LayoutBoxParam flexParam{
      .axis = Axis::VERTICAL,
      .crossAxisAlignment = param.crossAxisAlignment,
      .sizing = param.sizing,
      .childGap = param.childGap,
      .children = std::move(param.children),
  };
  return LayoutBox(flexParam);
}
