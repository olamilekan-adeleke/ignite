#include <fmt/base.h>

#include <memory>
#include <thread>

#include "component/component.hpp"
#include "component/state_base_component.hpp"
#include "elements/ui_view.hpp"
#include "foundation/color/color.hpp"
#include "foundation/geometry/axis.hpp"
#include "foundation/geometry/size.hpp"
#include "foundation/geometry/ui_edge_insets.hpp"
#include "foundation/icons/icon_types.hpp"
#include "foundation/utils/key.hpp"
#include "ui.hpp"
#include "ui_manager.hpp"

struct CounterState : public StateBase {
  ~CounterState() { cleanupResources(); }

  int count = 0;

  void initState() override {
    count = 0;
    stopTimer_ = false;

    timerThread_ = std::thread([this]() {
      while (!stopTimer_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        UIManager::instance().scheduleMainThreadTask([this]() {
          if (!stopTimer_) updateState([this]() { count++; });
        });
      }
    });
  }

  void dispose() override { cleanupResources(); }

  ComponentPtr build() override {
    const auto& style = TextStyle{.fontSize = 24};

    ComponentPtr text;
    if (count % 2 == 0) {
      text = IgniteUI::Paragraphs::text(fmt::format("Num {} is even", count), style, UIKey{"counter-even"});
    } else {
      text = IgniteUI::Paragraphs::text(fmt::format("Num {} is odd", count), style, UIKey{"counter-odd"});
    }

    const ViewParams param{
        .insets = UIEdgeInsets::horizontal(20) + UIEdgeInsets::vertical(12),
        .backgroundColor = Color::Red(),
        .borderRadius = 12,
        .onTap = [&](const UITapEvent& event) { printf("Tapped at (%f, %f)\n", event.x, event.y); },
    };
    const auto checkBoxSize = UISizing::Fixed(40, 40);

    return IgniteUI::center()({
        IgniteUI::Flex::column({.crossAxisAlignment = CrossAxisAlignment::CENTER, .childGap = 20})({
            IgniteUI::Image({.path = "assets/test_one.jpeg", .height = 350}),

            IgniteUI::Opacity({.opacity = 0.3})({
                IgniteUI::Paragraphs::icon({.icon = IconTypes::person(), .size = 40, .color = Color::Teal()}),
            }),

            text,

            count % 2 == 0
                ? IgniteUI::Paragraphs::text(fmt::format("Num 2X {} is even", count * 2), style, UIKey{"even-2"})
                : IgniteUI::Paragraphs::text(fmt::format("Num 2X {} is odd", count * 2), style, UIKey{"odd-2"}),

            IgniteUI::Separator({.axis = Axis::HORIZONTAL, .thinkness = 1, .backgroundColor = Color::Teal()}),

            IgniteUI::Paragraphs::text("Hello World"),

            IgniteUI::View(param)({
                IgniteUI::Paragraphs::text("Tap me!!"),
            }),

            IgniteUI::Flex::row({.childGap = 40, .crossAxisAlignment = CrossAxisAlignment::CENTER})({
                IgniteUI::CheckBox({.size = checkBoxSize}),
                IgniteUI::CheckBox({.size = checkBoxSize}),
            }),

            IgniteUI::TextField({
                .size = UISizing::GrowHeight(300),
                .onChanged = [this](const std::string& value) { fmt::println("IgniteUI::TextField {}", value); },
            }),

            IgniteUI::Paragraphs::text("End Of List"),
        }),
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
