#include <fmt/base.h>

#include <memory>
#include <vector>

#include "interactive_components/stateful_component.hpp"
#include "layout/flex_box.hpp"
#include "text_style.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "ui_edge_insets.hpp"

struct Todo {
  std::string text;
  bool done = false;

  Todo(std::string text) : text(text) {}

  void updateDone(bool value) { done = value; }
};

class TodoItemData {
  std::vector<Todo> items;

 public:
  TodoItemData() = default;

  void addTodoItem(std::string text) { items.push_back(Todo(text)); }

  std::vector<Todo> getItems() { return items; }

  void markDone(int index, bool value) { items[index].updateDone(value); }
};

class TodoListWidget : public StatefulComponent {
 public:
  TodoListWidget() {
    data.addTodoItem("Buy more coffee and monster");
    data.addTodoItem("Finish C++ project");
    // data.addTodoItem("Call dad");
  }

  std::shared_ptr<UIComponent> body() override {
    auto header = UI::VFlexBox({
        .children =
            {

                UI::Text("My Todo List", {.color = Color::Black(), .fontSize = 30, .weight = FontWeight::Bold}),
                UI::Text("List of today mini side quest to get done", {.color = Color::Gray(), .fontSize = 18}),
            },
    });

    return UI::UIView({
        .insets = UIEdgeInsets::horizontal(20) + UIEdgeInsets::vertical(30),
        .child = UI::VFlexBox({
            .spacing = 20,
            .crossAxisAlignment = CrossAxisAlignment::START,
            .children =
                {
                    header,
                    UI::UIView({.insets = UIEdgeInsets::all(10), .child = itemList()}),

                    makeTextField(),
                    button,
                },

        }),
    });
  }

 private:
  TodoItemData data = TodoItemData();
  std::string textFieldValue = "";

  const std::shared_ptr<UIComponent> makeTodoItem(int index, const std::string &label, bool done) {
    return UI::UIFlexBox({
        .spacing = 12,
        .axis = Axis::HORIZONTAL,
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .children =
            {
                UI::UICheckBox({
                    .enable = done,
                    .size = {24, 24},
                    .onTap =
                        [&, index, done](const UITapEvent &e) {
                          fmt::println("onTap: {}, {}", index, done);
                          data.markDone(index, !done);
                          markDirty();
                        },
                }),
                UI::Text(label,
                         {
                             .color = Color::Black(),
                             .fontSize = 18,
                             .decoration = done ? TextDecoration::strikethrough : TextDecoration::none,
                         }),
            },
    });
  }

  const std::shared_ptr<FlexBox> itemList() {
    auto buildChildren = [this]() {
      std::vector<std::shared_ptr<UIComponent>> children;
      for (int i = 0; i < data.getItems().size(); i++) {
        auto item = data.getItems()[i];
        children.push_back(makeTodoItem(i, item.text, item.done));
      }
      return children;
    };

    return UI::UIFlexBox({.spacing = 12, .axis = Axis::VERTICAL, .children = buildChildren()});
  }

  const std::shared_ptr<UIComponent> button = UI::UIButton({
      .child = UI::Text("Add Todo", {.color = Color::White(), .fontSize = 16}),
      .insets = UIEdgeInsets::horizontal(10) + UIEdgeInsets::vertical(10),
      .fillColor = Color::Blue(),
      .onTap =
          [this](const UITapEvent &e) {
            if (textFieldValue.empty()) return;

            data.addTodoItem(textFieldValue);
            textFieldValue.clear();
            markDirty();
          },
  });

  auto onTextFieldChanged(std::string &value) {
    fmt::println("onTextFieldChanged: {}", value);
    textFieldValue = value;
  }
  const std::shared_ptr<UIComponent> makeTextField() {
    return UI::UITextField(UITextFieldParams{
        .width = 0,
        .onChanged = [this](std::string value) { onTextFieldChanged(value); },
    });
  }
};
