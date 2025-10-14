#pragma once

#include <fmt/base.h>

#include <memory>

#include "elements/text_renderer.hpp"
#include "interactive_components/stateful_component.hpp"
#include "text_style.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "ui_edge_insets.hpp"

class ParagraphTestWidget : public StatefulComponent {
 public:
  ParagraphTestWidget() {}

  std::shared_ptr<UIComponent> body() override {
    return UI::UIView({
        .insets = UIEdgeInsets::horizontal(20) + UIEdgeInsets::vertical(30),
        .child = UI::Flex::column({
            .crossAxisAlignment = CrossAxisAlignment::START,
            .childGap = 20,
            .children =
                {
                    UI::Text("Paragraph Test", {.color = Color::Black(), .fontSize = 30, .weight = FontWeight::Bold}),
                    UI::Text("Testing different paragraph styles and long texts",
                             {.color = Color::Gray(), .fontSize = 18}),

                    UI::UIView({.margin = UIEdgeInsets::horizontal(100), .child = makePara()}),
                    UI::UIView({.margin = UIEdgeInsets::horizontal(50), .child = makeStyledPara()}),
                    UI::UIView({.margin = UIEdgeInsets::horizontal(20), .child = makeAnotherStyledPara()}),
                    UI::UIView({.margin = UIEdgeInsets::horizontal(20), .child = makeDecoratedText()}),
                },
        }),
    });
  }

 private:
  std::shared_ptr<UIComponent> makeDecoratedText() {
    auto text = "This is a decorated text with underline and strike-through.";
    const TextStyle params{
        .color = Color::Red(),
        .fontSize = 20,
        .weight = FontWeight::Bold,
        .decoration = TextDecoration::underline,
        .italic = true,
    };
    return std::make_shared<TextRenderer>(text, params);
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

  std::shared_ptr<UIComponent> makeStyledPara() {
    auto text =
        "This is another paragraph with a different style. It has a larger font size, "
        "a different color, and a bold weight. We are testing how the paragraph "
        "renderer handles various text styles and longer content. This should wrap "
        "nicely within the given constraints and demonstrate proper text layout. "
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
        "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
        "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
        "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat "
        "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    const TextStyle params{.color = Color::Blue(), .fontSize = 22, .weight = FontWeight::Bold};
    return std::make_shared<TextRenderer>(text, params);
  }

  std::shared_ptr<UIComponent> makeAnotherStyledPara() {
    auto text =
        "A third paragraph example, this time with a smaller font and a light gray color. "
        "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. "
        "This text is meant to test line spacing and general readability with a more subtle style. "
        "It should still be clear and well-formatted, even with a less prominent appearance. "
        "Testing multiple lines and ensuring that the text rendering engine handles various "
        "configurations correctly is crucial for a robust UI framework. We want to make sure "
        "that all text, regardless of its style or length, is displayed accurately and efficiently.";

    const TextStyle params{.color = Color::LightGray(), .fontSize = 14, .weight = FontWeight::Normal};
    return std::make_shared<TextRenderer>(text, params);
  }
};
