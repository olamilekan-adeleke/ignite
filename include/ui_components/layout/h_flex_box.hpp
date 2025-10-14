#pragma once

#include "layout/layout_box.hpp"

struct HFlexParam {
  float childGap = 0.0f;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  MainAxisSize sizing = MainAxisSize::FIT;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class HFlexBox {
 public:
  static LayoutBox create(const HFlexParam& param = {});
};
