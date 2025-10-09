#pragma once

#include <fmt/base.h>

#include <memory>
#include <string>
#include <vector>

#include "interactive_components/stateful_component.hpp"
#include "layout/scroll_view.hpp"
#include "size.hpp"
#include "ui.hpp"
#include "ui_edge_insets.hpp"

class TestScrollWidget : public StatefulComponent {
 public:
  TestScrollWidget() = default;

  std::shared_ptr<UIComponent> body() override {
    std::vector<std::shared_ptr<UIComponent>> children;

    for (int i = 0; i < 150; ++i) {
      int idx = i + 1;
      children.push_back(
          UI::Text("This is a very long line of text to test scrolling. Line number " + std::to_string(idx),
                   {
                       .color = Color::Black(),
                       .fontSize = 16,
                   }));

      if (i % 5 == 0) {
        std::vector<std::shared_ptr<UIComponent>> rowChildren;

        for (int i = 0; i < 2; ++i) {
          rowChildren.push_back(UI::UIView({
              .insets = UIEdgeInsets::all(50),
              .backgroundColor = i % 2 == 0 ? Color::Red() : Color::Blue(),
              .borderRadius = 10,
              .onTap = [&, i](const UITapEvent e) { fmt::println("Tapped {}", i); },
              .child = UI::Text(std::to_string(i + 1), {.color = Color::White()}),
          }));
        }

        children.push_back(UI::HFlexBox({.spacing = 40, .children = std::move(rowChildren)}));
      }
    }

    children.push_back(UI::FixedBoxView({.size = UISizing::GrowWidth(40)}));

    return UI::UIView({
        .insets = UIEdgeInsets::horizontal(20) + UIEdgeInsets{.top = 20},
        .child = std::make_shared<ScrollView>(ScrollViewParam{
            .child = UI::VFlexBox({.spacing = 10, .children = children}),
        }),
    });
  }
};
