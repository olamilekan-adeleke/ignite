#include <memory>

#include "text_style.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "ui_edge_insets.hpp"

#include "layout/flex_box.hpp"
#include "elements/text_field_renderer.hpp"
#include "interactive_components/stateful_component.hpp"

class TodoListWidget : public StatefulComponent {
 public:
  TodoListWidget() {}

  std::shared_ptr<UIComponent> body() override {
    const std::shared_ptr<FlexBox> itemList = UI::UIFlexBox({
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
    });

    const auto button = UI::UIButton({
        .child = UI::Text("Reset All Item", {.color = Color::White(), .fontSize = 16}),
        .onTap = [this](const UITapEvent& e) { markDirty(); },
    });

    return UI::UIView({
        .insets = UIEdgeInsets::horizonal(20) + UIEdgeInsets::vertical(30),
        .child = UI::VFlexBox({
            .spacing = 20,
            .crossAxisAlignment = CrossAxisAlignment::START,
            .children =
                {
                    UI::Text("My Todo List", {.color = Color::Black(), .fontSize = 30, .weight = FontWeight::Bold}),
                    UI::Text("List of today mini side quest to get done", {.color = Color::Gray(), .fontSize = 18}),

                    // UI::UIView({.insets = {.top = 20}, .child = UI::FixedBoxView({})}),
                    // itemList,

                    // UI::UIView({.insets = {.top = 20}, .child = UI::FixedBoxView({})}),
                    button,

                    // UI::UIView({.insets = {.top = 20}, .child = UI::FixedBoxView({})}),
                    // UI::UIView({.margin = UIEdgeInsets::horizonal(100), .child = makePara()}),

                    // UI::UIView({.insets = {.top = 20}, .child = UI::FixedBoxView({})}),
                    // UI::UIView({.insets = {.top = 20}, .child = UI::FixedBoxView({})}),
                    UI::UIView({.child = makeTextField()}),
                    makeTextField(),
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

  std::shared_ptr<UIComponent> makeTextField() {
    const UITextFieldParams param{};

    return std::make_shared<TextFieldRenderer>(param);
  }

  std::shared_ptr<UIComponent> makePara() {
    auto text =
        "Hello, world! 🙂🚀🔥🍕❤️🎉🐱🌍✨  This is a long test string with multiple \n languages and emojis. العربية: "
        "مرحبًا بك في الاختبار، كيف حالك اليوم؟ 中文 : 你好，世界！这是一个文本渲染测试字符串。 हिन्दी : नमस्ते, यह एक "
        "लंबा परीक्षण स्ट्रिंग है। Русский : Привет, это строка для тестирования рендеринга текста.日本語 : "
        "こんにちは、これはテキストレンダリングのテストです。  Yorùbá: Bawo ni, ayé! Èyí jẹ́ ìdánwò pípa ọrọ́. "
        "Accents & combining marks: é ã ö ů n̄ ṡ ž  Mixed directions: hello שלום مرحبا world 🌍  Numbers & symbols : "
        " 1234567890 @ #$ % ^&*()[]{ } < > / ? End of test ✅✨🔥 ";
    // text = "Hello word, this is a test";

    const TextStyle params{.color = Color::Black(), .fontSize = 18};
    return std::make_shared<TextRenderer>(text, params);
  }
};
