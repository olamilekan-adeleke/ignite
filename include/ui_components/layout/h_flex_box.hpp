#pragma once

#include "layout/flex_box.hpp"

struct HFlexParam {
  float spacing = 0.0f;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class HFlexBox {
 public:
  static FlexBox create(const HFlexParam& param = {});
};
