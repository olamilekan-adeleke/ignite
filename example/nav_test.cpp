#include <fmt/base.h>

#include <memory>
#include <string>
#include <vector>

#include "../example/layout/fitted_boxes.hpp"
#include "../example/paragraph_test.cpp"
#include "../example/test_scroll.cpp"
#include "../example/todo_list.cpp"
#include "basic/ui_component.hpp"
#include "color.hpp"
#include "elements/icon_renderer.hpp"
#include "icons/icon_types.hpp"
#include "interactive_components/stateful_component.hpp"
#include "navigation/navigataion_view.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "ui_edge_insets.hpp"

class NavTestWidget : public StatefulComponent {
 public:
  NavTestWidget() {
    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar(IconTypes::check(), "Todo List"),
        .view = std::make_shared<TodoListWidget>(),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar(IconTypes::home(), "Paragraph Test"),
        .view = std::make_shared<ParagraphTestWidget>(),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar(IconTypes::notifications(), "Scroll View Test"),
        .view = std::make_shared<TestScrollWidget>(),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar(IconTypes::add(), "Text Four"),
        .view = UI::UIView({.child = makePara()}),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar(IconTypes::settings(), "Layout Box one"),
        .view = UI::UIView({.child = LayoutBoxFixedBoxes::flexibleBody()}),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar(IconTypes::settings(), "Layout box two"),
        .view = UI::UIView({.child = LayoutBoxFixedBoxes::sizingBody()}),
    });
  }

  std::shared_ptr<UIComponent> body() override {
    return std::make_shared<NavigationView>(NavigationViewParam{
        .navigationItems = items_,
        .contentPadding = UIEdgeInsets::all(0),
    });
  }

 private:
  std::vector<NavigationItem> items_;

  std::shared_ptr<UIComponent> makeSideBar(const IconData& iconData, std::string text) const noexcept {
    return UI::Flex::row({
        .childGap = 10,
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .children =
            {
                std::make_shared<IconRenderer>(IconParam{.icon = iconData, .size = 20}),
                UI::UIView({
                    .insets = UIEdgeInsets{.top = 3},
                    .child = UI::Text(text, {.color = Color::Black()}),
                }),
                UI::FixedBoxView({.size = UISizing::GrowWidth(1)}),
            },

    });
  }

  std::shared_ptr<UIComponent> makePara() {
    auto text =
        "Hello, world! 🙂🚀🔥🍕❤️🎉🐱🌍✨  This is a long test string with multiple \n languages and emojis. العربية: "
        "مرحبًا بك في الاختبار، كيف حالك اليوم؟ 中文 : 你好，世界！这是一个文本渲染测试字符串。 हिन्दी : नमस्ते, यह एक "
        "लंबा परीक्षण स्ट्रिंग है। Русский : Привет, это строка для тестирования рендеринга текста.日本語 : "
        "こんにちは、これはテキストレンダリングのテストです。  Yorùbá: Bawo ni, ayé! Èyí jẹ́ ìdánwò pípa ọrọ́. "
        "Accents & combining marks: é ã ö ů n̄ ṡ ž  Mixed directions: hello שלום مرحبا world 🌍  Numbers & symbols : "
        "1234567890 @ #$ % ^&*()[]{ } < > / ? End of test ✅✨🔥 ";

    const TextStyle params{.color = Color::Black(), .fontSize = 18};

    std::vector<std::shared_ptr<UIComponent>> iconsList = {};
    const auto icons = {
        IconTypes::home(),
        IconTypes::search(),
        IconTypes::check(),
        IconTypes::close(),
        IconTypes::notifications(),
    };

    for (auto& icon : icons) {
      auto child = std::make_shared<IconRenderer>(IconParam{.icon = icon, .size = 40});
      iconsList.push_back(child);
    }

    return UI::Flex::column({
        .crossAxisAlignment = CrossAxisAlignment::CENTER,
        .childGap = 20,
        .children =
            {
                UI::Flex::row({.childGap = 20, .children = iconsList}),
                UI::Text(text, params),
            },
    });
  }
};
