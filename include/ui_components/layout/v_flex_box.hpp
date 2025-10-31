#pragma once

#include "foundation/utils/key.hpp"
#include "layout/layout_box.hpp"

struct VFlexParam {
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  MainAxisSize sizing = MainAxisSize::FIT;
  float childGap = 1.0f;
};

class VFlexBox {
 public:
  static LayoutBox create(const VFlexParam& param = {}, const UIKey& key = {});
};
