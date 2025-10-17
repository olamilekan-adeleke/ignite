#pragma once
#include <fmt/format.h>

#include <memory>

#include "basic/ui_component.hpp"
#include "size.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"

class LayoutBoxFixedBoxes {
 public:
  static std::shared_ptr<UIComponent> body() {
    // UI::Flex::row({
    //     .childGap = 10,
    //     .children =
    //         {
    //             UI::Text("Kepp Me Alive", TextStyle{.fontSize = 18}),
    //             UI::UICheckBox({.checked = true}),
    //         },
    // }),

    const auto& flex = UI::Flex::column({
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .childGap = 10,
        .children =
            {
                box(100, 100), box(150, 200), box(200, 50),

                // UI::Flex::row({
                //     .childGap = 20,
                //     .crossAxisAlignment = CrossAxisAlignment::CENTER,
                //     .children = {box(100, 100), box(100, 50), box(100, 120)},
                // }),
            },

    });

    return UI::UIView({
        .margin = UIEdgeInsets::all(20),
        .backgroundColor = Color::Red(),
        .child = flex,
    });
  }

  static std::shared_ptr<UIComponent> flexibleBody() {
    return UI::Flex::column({
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .childGap = 10,
        .children =
            {
                box(150, 100),
                flexBox(UISizing::Grow()),
                box(200, 100),
                flexBox(UISizing::GrowHeight(150)),
                box(150, 100),

                UI::Flex::row({
                    .childGap = 20,
                    .crossAxisAlignment = CrossAxisAlignment::CENTER,
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
    });
  }

  static std::shared_ptr<UIComponent> sizingBody() {
    return UI::Flex::column({
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        // .sizing = MainAxisSize::FILL,
        .childGap = 10,
        .children =
            {
                flexBox(UISizing::GrowHeight(150)),
                box(100, 100),
                UI::Text(fmt::format("This is Just A Random Text")),
                box(150, 200),
                UI::FixedBoxView({
                    .size = UISizing::GrowHeight(400),
                    .child = UI::Text(
                        fmt::format("That’s the hardest part of parsing Nigerian bank statements ⚡ — every bank has "
                                    "its own weird PDF layout. Some export a nice table (CSV-like), others dump it as "
                                    "text in blocks, some even render it as images. \nHere’s how you can reliably "

                                    "detect individual row items (transactions) after extracting PDF text:")),

                }),
                box(200, 50),

                UI::Flex::row({
                    .childGap = 20,
                    .crossAxisAlignment = CrossAxisAlignment::CENTER,
                    .children = {box(100, 100), box(100, 50), box(100, 120)},
                }),

            },
    });
  }

 private:
  static std::shared_ptr<UIComponent> box(float w, float h) {
    FixedBoxParam param{
        .size = UISizing::Fixed(w, h),
        .child = UI::Text(fmt::format("Fix Box {} x {}", w, h)),
    };
    return std::make_shared<FixedBox>(param);
  }

  static std::shared_ptr<UIComponent> flexBox(const UISizing& size) {
    FixedBoxParam param{
        .size = size,
        .child = UI::Text(fmt::format("Flexible Box")),
    };
    return std::make_shared<FixedBox>(param);
  }
};
