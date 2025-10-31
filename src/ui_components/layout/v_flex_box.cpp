#include "layout/v_flex_box.hpp"

#include "layout/layout_box.hpp"

LayoutBox VFlexBox::create(const VFlexParam& param, const UIKey& key) {
  LayoutBoxParam flexParam{
      .axis = Axis::VERTICAL,
      .crossAxisAlignment = param.crossAxisAlignment,
      .sizing = param.sizing,
      .childGap = param.childGap,
  };
  return LayoutBox(flexParam, key);
}
