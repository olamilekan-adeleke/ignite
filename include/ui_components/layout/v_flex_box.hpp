#pragma once

#include "layout/flex_box.hpp"

struct VFlexParam {
  float spacing = 1.0f;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class VFlexBox {
 public:
  static FlexBox create(const VFlexParam& param = {});
};
