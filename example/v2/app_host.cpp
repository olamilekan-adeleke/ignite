#include <fmt/base.h>

#include <memory>
#include <thread>

#include "component/component.hpp"
#include "component/state_base_component.hpp"
#include "elements/alignment_box.hpp"
#include "foundation/icons/icon_types.hpp"
#include "foundation/utils/key.hpp"
#include "ui.hpp"

struct CounterState : public StateBase {
  ~CounterState() { dispose(); }

  int count = 0;
  std::thread timerThread_;
  std::atomic<bool> stopTimer_{false};

  void initState() override {
    count = 0;
    stopTimer_ = false;

    timerThread_ = std::thread([this]() {
      while (!stopTimer_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!stopTimer_) updateState([this]() { count++; });
      }
    });
  }

  void dispose() override {
    stopTimer_ = true;
    if (timerThread_.joinable()) timerThread_.join();
  }

  ComponentPtr build() override {
    std::string string = R"(Hello, world! 🙂🚀🔥🍕❤️🎉🐱🌍✨ This is a long test string)";

    const auto& style = TextStyle{
        .fontSize = 24,
        // .textAlignment = TextAlignment::center,
    };

    // text = std::make_shared<Text>(string + "\n\nCount: " + std::to_string(count), );
    ComponentPtr text;
    if (count % 2 == 0) {
      text = IgniteUI::Paragraphs::text(fmt::format("The number {} is even", count), style, UIKey{"counter-even"});
    } else {
      text = IgniteUI::Paragraphs::text(fmt::format("The number {} is odd", count), style, UIKey{"counter-odd"});
    }

    const auto& icon = IgniteUI::Paragraphs::icon(IconParam{
        .icon = IconTypes::person(),
        .size = 40,
    });
    return IgniteUI::Position::center(icon);
    // return std::make_shared<AlignmentBox>(text);
  }
};

class Counter : public StatefullyComponent {
 public:
  Counter() : StatefullyComponent() {}

  std::shared_ptr<StateBase> createState() override { return std::make_shared<CounterState>(); }
};
