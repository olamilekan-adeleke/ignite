#include "ui.hpp"
#include "ui_alignment.hpp"
#include "ui_component.hpp"
#include "stateful_component.hpp"

class CounterComponent : public StatefulComponent {
 public:
  CounterComponent() : count_(0) {}

  // clang-format off
  std::shared_ptr<UIComponent> body() override {

    auto firstColum = UI::ColumnView({
      // .spacing = 10,
      .mainAxisAlignment = MainAxisAlignment::CENTER,
      .crossAxisAlignment = CrossAxisAlignment::CENTER,
      .crossAxisSize = CrossAxisSize::FILL,
      .mainAxisSize = MainAxisSize::FILL,
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
    });

    auto thirdColum = UI::ColumnView({
      .spacing = 10,
      .crossAxisAlignment = CrossAxisAlignment::CENTER,
      .crossAxisSize = CrossAxisSize::FILL,
      .mainAxisSize = MainAxisSize::FILL,
      .children = {
        UI::Text("Count 3: " + std::to_string(count_), { 
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
    });

    // auto secondColum = UI::ColumnView({
    //   .spacing = 10,
    //   .crossAxisAlignment = CrossAxisAlignment::CENTER,
    //   .crossAxisSize = CrossAxisSize::FIT,
    //   .mainAxisSize = MainAxisSize::FILL,
    //   .children = {
    //     UI::Text("Count 2: " + std::to_string(count_), { 
    //       .color = Color::Black(), 
    //       .fontSize = 50, 
    //       .weight = FontWeight::Bold, 
    //     }),
    //
    //     UI::UIView({ 
    //       .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
    //       .backgroundColor = Color::Blue(), 
    //       .borderRadius = 5, 
    //       .onTap = [this](const UITapEvent&) { 
    //         count_++;
    //         markDirty();
    //       }, 
    //       .child = UI::Text("++", {.color = Color::White(), .fontSize = 25}),
    //     }), 
    //   }, 
    // });


    return UI::UIView({ 
    // .margin = {.left = 40},
    // .backgroundColor = Color::Gray(),
    .child = UI::ColumnView({
        .spacing = 10,
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .crossAxisSize = CrossAxisSize::FILL,
        .mainAxisSize = MainAxisSize::FILL,
        .children = {
          UI::UIView({
            .backgroundColor = Color::Magenta().withAlpha(255 * 0.2),
            .child = firstColum,
          }),
          // UI::UIView({
          //   .backgroundColor = Color::Cyan().withAlpha(255 * 0.2),
          //   .child = thirdColum,
          // }),
          // UI::UIView({
          //   .backgroundColor = Color::Yellow().withAlpha(255 * 0.2),
          //   // .child = secondColum,
          // }),
        },
      })
    });
  }
  // clang-format on

 private:
  int count_;
};
