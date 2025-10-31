#include "layout/h_flex_box.hpp"

#include "layout/layout_box.hpp"

LayoutBox HFlexBox::create(const HFlexParam& param, const UIKey& key) {
  LayoutBoxParam flexParam{
      .axis = Axis::HORIZONTAL,
      .crossAxisAlignment = param.crossAxisAlignment,
      .sizing = param.sizing,
      .childGap = param.childGap,
  };
  return LayoutBox(flexParam, key);
}
