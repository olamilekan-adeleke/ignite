#include "layout/v_flex_box.hpp"
#include "layout/flex_box.hpp"

FlexBox VFlexBox::create(const VFlexParam& param) {
  FlexParam flexParam{
      .spacing = param.spacing,
      .axis = Axis::VERTICAL,
      .crossAxisAlignment = param.crossAxisAlignment,
      .children = param.children,
  };
  return FlexBox(flexParam);
}
