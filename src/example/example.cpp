#include <fmt/base.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "logger.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"

struct TodoItem {
  std::string title = "To study React fundamentals";
  bool isDone = false;

  TodoItem(std::string t, bool done = false) : title(std::move(t)), isDone(done) {}

  TodoItem() = default;

  static std::vector<TodoItem> todoItems() {
    std::vector<TodoItem> items;
    items.emplace_back("Learn C++ advanced concepts", false);
    items.emplace_back("Practice data structures", false);
    items.emplace_back("Finish project documentation", false);
    items.emplace_back("Buy groceries", true);
    items.emplace_back("Go for a run", false);
    items.emplace_back("Ligma my Balls", false);

    return items;
  }
};

inline const std::vector<TodoItem> myTodoList = TodoItem::todoItems();

// clang-format off
inline const auto headerView = UI::HFlexBox({
  .spacing = 11,
  .crossAxisAlignment = CrossAxisAlignment::CENTER,
  .children = {
    UI::UIView({
      .backgroundColor = Color(0x9E78CF).withAlpha(255 * 0.2),
      .borderRadius = 10.0F,
      .child = UI::FixedBoxView({.size = {.width = 380, .height = 40}})
    }),
    UI::UIView({
      .insets = {.top = 6, .left = 6, .right = 6, .bottom = 6},
      .backgroundColor = Color(0x9E78CF).withAlpha(255),
      .borderRadius = 10.0F,
      .antiAlias = true,
      .child = UI::UIImageView({
        .path = "assets/plus.png",
        .width = 20, 
        .height = 20,
      }),
    }),
  },
});

inline auto buildTodoItem(const TodoItem& item) {

  std::shared_ptr<UIComponent> checkBox = item.isDone  ?
    UI::FixedBoxView({}) :  
    UI::UIImageView({ .path = "assets/Check.png", .width = 22, .height = 22 });

  return UI::UIView({
    .insets = {.top = 28, .left = 20, .bottom = 28, .right = 20},
    .backgroundColor = Color(0x15101C).withAlpha(255),
    .borderRadius = 10.0F,
    .antiAlias = false,
    .child = UI::FixedBoxView({
      .size = {.width = 432},
      .child = UI::HFlexBox({
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .children = {
          UI::Text(item.title, {
            .color = item.isDone ? Color(0xf78CFB0).withAlpha(255) : Color(0x9E78CF).withAlpha(255),
            .fontSize = 16,
            .weight = FontWeight::Normal,
          }),
          UI::HFlexBox({
            .spacing = 8,
            .children = {
              checkBox,
              UI::UIView({
                .backgroundColor = Color(0x15101C).withAlpha(255),
                .tappable = true,
                .onTap = [item](const UITapEvent& event) {
                  auto it = std::find_if(
                    myTodoList.begin(), myTodoList.end(),
                    [item](const TodoItem& ti) { return ti.title == item.title;}
                  );
                  if (it != myTodoList.end()){
                    // myTodoList.erase(it);
                    fmt::println("Deleted item {}", item.title.c_str());
                  }
                },
                .child = UI::UIImageView({ .path = "assets/trash.png" }),
              }),
            },
          }),
        },
      }),
    }),
  });
}

inline auto bodyTodoListItems() {
  Logger::log("Drawing bodyTodoListItems ..");

  std::vector<std::shared_ptr<UIComponent>> children;
  for (const auto& item : myTodoList) {
    if (item.isDone) {
      children.push_back(buildTodoItem(item));
    }
  }

  return UI::VFlexBox({
    .crossAxisAlignment = CrossAxisAlignment::START,
    .children = {
      UI::UIView({
        .onTap = [](const UITapEvent& event){
           fmt::println("Tap item at item {}");
        },
        .child = UI::Text("Tasks to do - " + std::to_string(children.size()), {
        .color = Color::White(),
        .fontSize = 16,
        .weight = FontWeight::Normal,
      }),
      }),
      UI::FixedBoxView({.size = {.height = 18}}),
      UI::VFlexBox({
        .spacing = 16,
        .crossAxisAlignment = CrossAxisAlignment::START,
        .children = std::move(children),
      }),
    },
  });
}

inline auto bodySuccessListItems() {
  std::vector<std::shared_ptr<UIComponent>> children;
  for (const auto& item : myTodoList) {
    if (!item.isDone) {
      children.push_back(buildTodoItem(item));
    };
  }

  return UI::VFlexBox({
    .crossAxisAlignment = CrossAxisAlignment::START,
    .children = {
      UI::Text("Done - " + std::to_string(children.size()), {
        .color = Color::White(),
        .fontSize = 16,
        .weight = FontWeight::Normal,
      }),
      UI::FixedBoxView({.size = {.height = 18}}),
      UI::VFlexBox({
        .spacing = 16,
        .crossAxisAlignment = CrossAxisAlignment::START,
        .children = std::move(children),
      }),
    },
  });
}

inline const auto rootApp = UI::UIView({
  .backgroundColor = Color::Black(),
  .child = UI::UIView({
    .insets = {.top = 50, .left = 65, .right = 65, .bottom = 50},
    .margin = {.top = 80, .left = 40, .bottom = 80, .right = 40},
    .backgroundColor = Color(0x1D1825).withAlpha(255),
    .borderRadius = 20.0f,
    .antiAlias = true,
    .child = UI::VFlexBox({
      .children = {
        headerView,
        UI::FixedBoxView({.size = {.height = 50}}),
        bodyTodoListItems(),
        UI::FixedBoxView({.size = {.height = 50}}),
        bodySuccessListItems(),
      },
    }),
  }),
});
