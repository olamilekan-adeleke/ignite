#pragma once

#include "layout/layout_box.hpp"

struct VFlexParam {
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  MainAxisSize sizing = MainAxisSize::FIT;
  float childGap = 1.0f;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class VFlexBox {
 public:
  static LayoutBox create(const VFlexParam& param = {});
};
