
#include <fmt/format.h>

#include <memory>

#include "basic/ui_component.hpp"
#include "elements/fixed_box.hpp"
#include "layout/layout_box.hpp"
#include "size.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"

class LayoutBoxFixedBoxes {
 public:
  static std::shared_ptr<UIComponent> body() {
    LayoutBoxParam param{
        .axis = Axis::VERTICAL,
        .childGap = 10,
        .children =
            {
                box(100, 100),
                box(150, 200),
                box(200, 50),

                std::make_shared<LayoutBox>(LayoutBoxParam{
                    .axis = Axis::HORIZONTAL,
                    .childGap = 20,
                    .children = {box(100, 100), box(100, 100), box(100, 100)},
                }),
            },
    };
    return std::make_shared<LayoutBox>(param);
  }

  static std::shared_ptr<UIComponent> flexibleBody() {
    LayoutBoxParam param{
        .axis = Axis::VERTICAL,
        .childGap = 10,
        .children =
            {
                box(150, 100),
                flexBox(UISizing::Grow()),
                box(200, 100),
                flexBox(UISizing::GrowHeight(150)),
                box(150, 100),

                std::make_shared<LayoutBox>(LayoutBoxParam{
                    .axis = Axis::HORIZONTAL,
                    .childGap = 20,
                    .children =
                        {
                            box(150, 100),
                            flexBox(UISizing::GrowWidth(100)),
                            box(150, 100),
                            flexBox(UISizing::GrowWidth(100)),
                            box(150, 100),
                        },
                }),
            },
    };
    return std::make_shared<LayoutBox>(param);
  }

 private:
  static std::shared_ptr<UIComponent> box(float w, float h) {
    FixedBoxParam param{
        .size = UISizing::Fixed(w, h),
        .child = UI::Text(fmt::format("Fix Box {} x {}", w, h)),
        .color = Color::Random(),
    };
    return std::make_shared<FixedBox>(param);
  }

  static std::shared_ptr<UIComponent> flexBox(const UISizing& size) {
    FixedBoxParam param{
        .size = size,
        .child = UI::Text(fmt::format("Flexible Box")),
        .color = Color::Random(),
    };
    return std::make_shared<FixedBox>(param);
  }
};
