#include "layout/h_flex_box.hpp"
#include "layout/flex_box.hpp"

FlexBox HFlexBox::create(const HFlexParam& param) {
  FlexParam flexParam{
      .spacing = param.spacing,
      .axis = Axis::HORIZONTAL,
      .crossAxisAlignment = param.crossAxisAlignment,
      .children = param.children,
  };
  return FlexBox(flexParam);
}
