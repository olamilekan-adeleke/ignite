#include <fmt/base.h>

#include <memory>
#include <thread>

#include "component/component.hpp"
#include "component/state_base_component.hpp"
#include "foundation/geometry/axis.hpp"
#include "foundation/icons/icon_types.hpp"
#include "foundation/utils/key.hpp"
#include "ui.hpp"

struct CounterState : public StateBase {
  ~CounterState() { cleanupResources(); }

  int count = 0;

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

  void dispose() override { cleanupResources(); }

  ComponentPtr build() override {
    const auto& style = TextStyle{.fontSize = 24};

    ComponentPtr text;
    if (count % 2 == 0) {
      text = IgniteUI::Paragraphs::text(fmt::format("The number {} is even", count), style, UIKey{"counter-even"});
    } else {
      text = IgniteUI::Paragraphs::text(fmt::format("The number {} is odd", count), style, UIKey{"counter-odd"});
    }

    return IgniteUI::Box({.size = UISizing::Fixed(400, 400), .alignment = UIAlignment::Center})({
        IgniteUI::Separator({.axis = Axis::HORIZONTAL, .thinkness = 4, .backgroundColor = Color::Teal()}),

        IgniteUI::Image({.path = "assets/test_one.jpeg", .width = 200, .height = 200}),

        IgniteUI::Opacity({.opacity = 0.3})({
            IgniteUI::Paragraphs::icon({.icon = IconTypes::person(), .size = 40, .color = Color::Teal()}),
        }),

        IgniteUI::Paragraphs::text("Hello World"),
    });
  }

 private:
  std::thread timerThread_;
  std::atomic<bool> stopTimer_{false};
  bool disposed_ = false;

  void cleanupResources() {
    if (disposed_) return;
    stopTimer_ = true;
    if (timerThread_.joinable()) timerThread_.join();
    disposed_ = true;
  }
};

class Counter : public StatefullyComponent {
 public:
  Counter() : StatefullyComponent() {}

  std::shared_ptr<StateBase> createState() override { return std::make_shared<CounterState>(); }
};
