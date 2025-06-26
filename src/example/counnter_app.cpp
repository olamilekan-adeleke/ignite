#include "size.hpp"
#include "ui.hpp"
#include "ui_component.hpp"
#include "stateful_component.hpp"

class CounterComponent : public StatefulComponent {
 public:
  CounterComponent() : count_(0) {}

  // clang-format off
  std::shared_ptr<UIComponent> body() override {
    return UI::UIView({ 
    .margin = {.left = 40},
    .backgroundColor = Color::Gray(),
    .child = UI::ColumnView({
      .spacing = 10,
      .crossAxisAlignment = CrossAxisAlignment::START,
      .children = {
        UI::Text("Count: " + std::to_string(count_), { 
          .color = Color::Black(), 
          .fontSize = 50, 
          .weight = FontWeight::Bold, 
        }),

        UI::UIView({ 
          .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
          .backgroundColor = Color::Green(), 
          .borderRadius = 5, 
          .onTap = [this](const UITapEvent&) { 
            count_++;
            markDirty();
          }, 
          .child = UI::Text("+", {.color = Color::White(), .fontSize = 25}),
        }), 
      }, 
    }),
    });
  }
  // clang-format on

 private:
  int count_;
};
