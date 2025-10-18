#pragma once
#include <fmt/base.h>

#include <memory>

#include "interactive_components/stateful_component.hpp"
#include "text_style.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "ui_edge_insets.hpp"

struct CounterData {
  int count = 0;

  void increment() { count++; }
  void decrement() { count--; }
};

class CounterWidget : public StatefulComponent {
 public:
  CounterWidget() = default;

  std::shared_ptr<UIComponent> body() override {
    return UI::UIView({
        .insets = UIEdgeInsets::vertical(20) + UIEdgeInsets::horizontal(16),
        .child = UI::Flex::column({
            .crossAxisAlignment = CrossAxisAlignment::CENTER,
            // .sizing = MainAxisSize::FILL,
            .childGap = 20,
            .children =
                {
                    // UI::FixedBoxView({.size = UISizing::GrowHeight(1)}),
                    makeCounterText(),
                    makeButtonsRow(),
                    // UI::FixedBoxView({.size = UISizing::GrowHeight(1)}),
                },
        }),
    });
  }

 private:
  CounterData data;

  std::shared_ptr<UIComponent> makeCounterText() {
    return UI::Text(fmt::format("Count: {}", data.count),
                    {
                        .color = Color::Black(),
                        .fontSize = 50,
                        .weight = FontWeight::Bold,
                    });
  }

  std::shared_ptr<UIComponent> makeAddButton() {
    return UI::UIButton({
        .child = UI::FixedBoxView({
            .size = UISizing(160, 40),
            .child = UI::Text("Add", {.color = Color::White(), .fontSize = 18}),
        }),
        .insets = UIEdgeInsets::all(0),
        .fillColor = Color::Blue(),
        .onTap =
            [this](const UITapEvent &e) {
              data.increment();
              markDirty();
            },
    });
  }

  std::shared_ptr<UIComponent> makeRemoveButton() {
    return UI::UIButton({
        .child = UI::FixedBoxView({
            .size = UISizing(160, 40),
            .child = UI::Text("Remove", {.color = Color::White(), .fontSize = 18}),
        }),
        .insets = UIEdgeInsets::all(0),
        .fillColor = Color::Red(),
        .onTap =
            [this](const UITapEvent &e) {
              data.decrement();
              markDirty();
            },
    });
  }

  std::shared_ptr<UIComponent> makeButtonsRow() {
    return UI::Flex::row({
        .childGap = 20,
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        // .sizing = MainAxisSize::FILL,
        .children =
            {
                // UI::FixedBoxView({.size = UISizing::GrowWidth(1)}),
                makeAddButton(),
                makeRemoveButton(),
                // UI::FixedBoxView({.size = UISizing::GrowWidth(1)}),
            },
    });
  }
};
