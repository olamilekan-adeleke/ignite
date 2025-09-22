#include "layout/flex_box.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "stateful_component.hpp"

class TodoListWidget : public StatefulComponent {
 public:
  TodoListWidget() {}

  std::shared_ptr<UIComponent> body() override {
    return UI::UIView({
        .insets = UIEdgeInsets{30, 20, 0, 20},
        .child = UI::VFlexBox({
            .spacing = 6,
            .crossAxisAlignment = CrossAxisAlignment::START,
            .children =
                {
                    UI::Text("My Todo List", {.color = Color::Black(), .fontSize = 30, .weight = FontWeight::Bold}),
                    UI::Text("List of today mini side quest to get done", {.color = Color::Gray(), .fontSize = 18}),

                    UI::UIView({.insets = {.top = 20}, .child = UI::SizedView({})}),

                    UI::UIFlexBox({
                        .spacing = 12,
                        .axis = Axis::VERTICAL,
                        .children =
                            {
                                makeTodoItem("Buy more coffec and monster"),
                                makeTodoItem("Finish C++ project"),
                                makeTodoItem("Call dad"),
                                makeTodoItem("Read 30 pages of a book"),
                                makeTodoItem("Don't forget to ragebait hb"),
                            },
                    }),
                },
        }),

    });
  }

 private:
  std::shared_ptr<UIComponent> makeTodoItem(const std::string& label) {
    return UI::UIFlexBox({
        .spacing = 12,
        .axis = Axis::HORIZONTAL,
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .children =
            {
                UI::UICheckBox({.size = {24, 24}}),
                UI::Text(label, {.color = Color::Black(), .fontSize = 18}),
            },
    });
  }
};
