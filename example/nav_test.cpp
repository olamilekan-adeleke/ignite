#include <fmt/base.h>

#include <memory>
#include <string>
#include <vector>

#include "../example/paragraph_test.cpp"
#include "../example/test_scroll.cpp"
#include "../example/todo_list.cpp"
#include "basic/ui_component.hpp"
#include "color.hpp"
#include "interactive_components/stateful_component.hpp"
#include "navigation/navigataion_view.hpp"
#include "ui.hpp"
#include "ui_edge_insets.hpp"

class NavTestWidget : public StatefulComponent {
 public:
  NavTestWidget() {
    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar("Paragraph Test"),
        .view = std::make_shared<ParagraphTestWidget>(),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar("Todo List"),
        .view = std::make_shared<TodoListWidget>(),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar("Scroll View Test"),
        .view = std::make_shared<TestScrollWidget>(),
    });

    items_.push_back(NavigationItem{
        .sidebarItem = makeSideBar("Text Four"),
        .view = UI::UIView({.child = makePara()}),
    });
  }

  std::shared_ptr<UIComponent> body() override {
    //
    return std::make_shared<NavigationView>(NavigationViewParam{
        .navigationItems = items_,
        .contentPadding = UIEdgeInsets::all(0),
    });
  }

 private:
  std::vector<NavigationItem> items_;

  std::shared_ptr<UIComponent> makeBody(Color color) const noexcept {
    return UI::UIView({
        .insets = UIEdgeInsets::all(100),
        .backgroundColor = color,
    });
  }

  std::shared_ptr<UIComponent> makeSideBar(std::string text) const noexcept {
    return UI::Text(text, {.color = Color::Black()});
  }

  std::shared_ptr<UIComponent> makePara() {
    auto text =
        "Hello, world! 🙂🚀🔥🍕❤️🎉🐱🌍✨  This is a long test string with multiple \n languages and emojis. العربية: "
        "مرحبًا بك في الاختبار، كيف حالك اليوم؟ 中文 : 你好，世界！这是一个文本渲染测试字符串。 हिन्दी : नमस्ते, यह एक "
        "लंबा परीक्षण स्ट्रिंग है। Русский : Привет, это строка для тестирования рендеринга текста.日本語 : "
        "こんにちは、これはテキストレンダリングのテストです。  Yorùbá: Bawo ni, ayé! Èyí jẹ́ ìdánwò pípa ọrọ́. "
        "Accents & combining marks: é ã ö ů n̄ ṡ ž  Mixed directions: hello שלום مرحبا world 🌍  Numbers & symbols : "
        " 1234567890 @ #$ % ^&*()[]{ } < > / ? End of test ✅✨🔥 ";

    const TextStyle params{.color = Color::Black(), .fontSize = 18};
    return std::make_shared<TextRenderer>(text, params);
  }
};
