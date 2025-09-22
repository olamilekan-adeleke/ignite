#include <memory>
#include "layout/flex_box.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "stateful_component.hpp"

class CounterTextWidget : public StatefulComponent {
 public:
  CounterTextWidget() : count_(0) {}

  // clang-format off
  std::shared_ptr<UIComponent> body() override {

    return UI::VFlexBox({
      .spacing = 10,
      .crossAxisAlignment = CrossAxisAlignment::CENTER,
      .children = {
            UI::UIView({ 
            .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
            .margin = {.top = 50},
            .backgroundColor = Color::Red(), 
            .borderRadius = 5, 
            .onTap = [this](const UITapEvent&) { 
              count_++;
              markDirty();
            }, 
            .child = UI::Text("Click Me", {.color = Color::White(), .fontSize = 25}),
          }), 


        UI::Text(
          "Lorem Ipsum is simply dummy text of the printing and typesetting industry Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.",
          { 
          .color = Color::Black(), 
          .fontSize = 32, 
          .weight = FontWeight::Bold, 
        }),


        UI::Text(
          "Tap Count: " + std::to_string(count_),
          { 
          .color = Color::Magenta(), 
          .fontSize = 32, 
          .weight = FontWeight::Bold, 
        }),

      UI::UIFlexBox({
          .spacing = 40,
          .axis = Axis::HORIZONTAL,
          .crossAxisAlignment = CrossAxisAlignment::START,
          .children = {
              UI::UICheckBox({
                  .size = {25, 25},
              }),
              UI::UICheckBox({
                  .size = {25, 25},
              }),
          },
          }),


        UI::UIFlexBox({
          .spacing = 20,
          .axis = Axis::HORIZONTAL,
          .children = {

        UI::UIImageView({
          .path = "assets/test_one.jpeg",
          .width = 320,
          .height = 320,
        }),

        UI::OpacityView({
            .opacity = 0.3f,
            .child = UI::UIView({ 
                .borderRadius = 320.0 / 2.0f,
                .child = UI::UIImageView({
                .path = "assets/user_one.jpeg",
                .width = 320,
                .height = 320,
              }),
            }),
        }),

        }
        }),

        UI::UISeparator({
          .axis = Axis::HORIZONTAL,
          .thinkness = 1.0f,
        }),

     // UI::UIFlexBox({
     //  .spacing = 20,
     //  .crossAxisAlignment = CrossAxisAlignment::START,
     //  .children = {
     //        UI::UIView({ 
     //        .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
     //        .backgroundColor = Color::Green(), 
     //        .borderRadius = 5, 
     //        .onTap = [this](const UITapEvent&) { 
     //          count_++;
     //          markDirty();
     //        }, 
     //        .child = UI::Text("+", {.color = Color::White(), .fontSize = 25}),
     //      }), 
     //
     //
     //        UI::UIView({ 
     //        .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
     //        .backgroundColor = Color::Green(), 
     //        .borderRadius = 5, 
     //        .child = UI::Text("+", {.color = Color::White(), .fontSize = 25}),
     //      }), 
     //
     //        UI::UIView({ 
     //        .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
     //        .backgroundColor = Color::Green(), 
     //        .borderRadius = 5, 
     //        .onTap = [this](const UITapEvent&) { 
     //          count_++;
     //          markDirty();
     //        }, 
     //        .child = UI::Text("+", {.color = Color::White(), .fontSize = 25}),
     //      }), 
     //   }
     //   }),

        UI::UIFlexBox({
        // UI::HFlexBox({
          .spacing = 40,
          .axis = Axis::HORIZONTAL,
          .crossAxisAlignment = CrossAxisAlignment::START,
          .children = {
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

          UI::UIView({ 
            .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
            .backgroundColor = Color::Red(), 
            .borderRadius = 5, 
            .onTap = [this](const UITapEvent&) { 
              count_--;
              markDirty();
            }, 
            .child = UI::Text("-", {.color = Color::White(), .fontSize = 25}),
          }), 
          }
        }), 
      }, 
    });
  }
  // clang-format on

 private:
  int count_;
};

class CounterComponent : public StatefulComponent {
 public:
  CounterComponent() {}

  // clang-format off
  std::shared_ptr<UIComponent> body() override {

    auto thirdColum = UI::VFlexBox({
      .spacing = 10,
      .crossAxisAlignment = CrossAxisAlignment::CENTER,
      .children = {
        UI::Text("Count 3: ", { 
          .color = Color::Black(), 
          .fontSize = 50, 
          .weight = FontWeight::Bold, 
        }),

        UI::UIView({ 
          .insets = {.top = 16, .left = 16, .bottom = 16, .right = 16}, 
          .backgroundColor = Color::Green(), 
          .borderRadius = 5, 
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

    auto firstColum = std::make_shared<CounterTextWidget>();
    return firstColum;

    // return UI::UIView({ 
    // // .margin = {.left = 40, .top = 40},
    // // .backgroundColor = Color::Gray(),
    // .child = UI::ColumnView({
    //     // .spacing = 10,
    //     // .crossAxisAlignment = CrossAxisAlignment::CENTER,
    //     // .crossAxisSize = CrossAxisSize::FILL,
    //     // .mainAxisSize = MainAxisSize::FILL,
    //     .children = {
    //       UI::UIView({
    //         .backgroundColor = Color::Yellow().withAlpha(255 * 0.2),
    //         .child = firstColum,
    //       }),
    //       // UI::UIView({
    //       //   .backgroundColor = Color::Cyan().withAlpha(255 * 0.2),
    //       //   .child = thirdColum,
    //       // }),
    //       // UI::UIView({
    //       //   .backgroundColor = Color::Yellow().withAlpha(255 * 0.2),
    //       //   // .child = secondColum,
    //       // }),
    //     },
    //   })
    // });
  }
  // clang-format on
};
