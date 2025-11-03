#include <fmt/base.h>

#include "animation/animation.hpp"
#include "animation/curves.hpp"
#include "component/component.hpp"
#include "component/state_base_component.hpp"
#include "foundation/color/color.hpp"
#include "foundation/geometry/ui_alignment.hpp"
#include "foundation/utils/key.hpp"
#include "ui.hpp"

using namespace IgniteAnimation;

struct AnimationExampleStateAdvanced : public StateBase {
  AnimationExampleStateAdvanced() = default;
  ~AnimationExampleStateAdvanced() { boxWidth_.stop(); }

  void initState() override {
    boxWidth_.setCurve(Curves::easeInQuad);

    boxWidth_.setOnUpdate([this](double eased) {
      const auto val = boxWidth_.getValue();
      updateState([&]() { currentBoxWidth_ = val; });
    });

    boxWidth_.setOnComplete([this]() { fmt::println("boxWidth_ Animation completed!!"); });

    rotation_.setOnUpdate([this](double eased) {
      const auto val = rotation_.getValue();
      updateState([&]() { currentRotation_ = val; });
    });

    scaling_.setOnUpdate([this](double eased) {
      const auto val = scaling_.getValue();
      updateState([&]() { currentScaling_ = val; });
    });

    moxingX_.setOnUpdate([this](double eased) {
      const auto val = moxingX_.getValue();
      updateState([&]() { currentMoxingX_ = val; });
    });
  }

  ComponentPtr build() override {
    const ViewParams param1{
        .insets = UIEdgeInsets::horizontal(20) + UIEdgeInsets::vertical(12),
        .backgroundColor = Color::Green(),
        .borderRadius = 12,
        .onTap = [this](const UITapEvent& event) { boxWidth_.play(); },
    };

    const ViewParams param{
        .insets = UIEdgeInsets::horizontal(20) + UIEdgeInsets::vertical(12),
        .backgroundColor = Color::Red(),
        .borderRadius = 12,
        .onTap = [this](const UITapEvent& event) { boxWidth_.reverse(); },
    };

    return IgniteUI::center()({
        IgniteUI::Flex::column({.crossAxisAlignment = CrossAxisAlignment::CENTER, .childGap = 20})({
            IgniteUI::Paragraphs::text("Animation Example"),

            IgniteUI::View({.backgroundColor = Color::Teal(), .borderRadius = 20})({
                IgniteUI::Box({.size = UISizing(currentBoxWidth_, 100)})({
                    IgniteUI::Paragraphs::text("Growing"),
                }),
            }),

            IgniteUI::Paragraphs::text("Rotate Transform"),
            IgniteUI::transform({.transform = IgniteTransform::Matrix3::rotation(currentRotation_ * M_PI / 180)})({
                IgniteUI::View({.backgroundColor = Color::Teal(), .borderRadius = 20})({
                    IgniteUI::Box({.size = UISizing(150, 100)})({
                        IgniteUI::Paragraphs::text("Rotating"),
                    }),
                }),
            }),

            IgniteUI::Paragraphs::text("Scale Transform"),
            IgniteUI::transform({.transform = IgniteTransform::Matrix3::scaling(currentScaling_, currentScaling_)})({
                IgniteUI::View({.backgroundColor = Color::Teal(), .borderRadius = 20})({
                    IgniteUI::Box({.size = UISizing(150, 100)})({
                        IgniteUI::Paragraphs::text("Scaling"),
                    }),
                }),
            }),

            IgniteUI::Paragraphs::text("Scale And Rotate Transform"),
            IgniteUI::transform({
                .transform = IgniteTransform::Matrix3::scaling(currentScaling_, currentScaling_) *
                             IgniteTransform::Matrix3::rotation(currentRotation_ * M_PI / 180),
            })({
                IgniteUI::View({.backgroundColor = Color::Teal(), .borderRadius = 20})({
                    IgniteUI::Box({.size = UISizing(150, 100)})({
                        IgniteUI::Paragraphs::text("Scaling & Rotating"),
                    }),
                }),
            }),

            IgniteUI::Paragraphs::text("Move Transform"),
            IgniteUI::transform({.transform = IgniteTransform::Matrix3::translation(currentMoxingX_, 0)})({
                IgniteUI::View({.backgroundColor = Color::Teal(), .borderRadius = 20})({
                    IgniteUI::Box({.size = UISizing(150, 100)})({
                        IgniteUI::Paragraphs::text("Scaling"),
                    }),
                }),
            }),

            IgniteUI::Flex::row({.childGap = 40})({
                buildButton("Start", [this](const UITapEvent& event) { startAllAnimations(); }),
                buildButton("Reverse", [this](const UITapEvent& event) { reverseAllAnimations(); }),
            }),
        }),
    });
  }

  void startAllAnimations() {
    boxWidth_.play();
    rotation_.play();
    scaling_.play();
    moxingX_.play();
  }

  void reverseAllAnimations() {
    boxWidth_.reverse();
    rotation_.reverse();
    scaling_.reverse();
    moxingX_.reverse();
  }

 private:
  Animation<float> boxWidth_{100.0f, 450.0f, 2.0};
  Animation<float> rotation_{0.0f, 300.0f, 2.0};
  Animation<float> scaling_{1.0f, 0.2f, 2.0};
  Animation<float> moxingX_{0.0f, 100.0f, 2.0};

  float currentBoxWidth_ = 100.0f;
  float currentRotation_ = 0.0f;
  float currentScaling_ = 1.0f;
  float currentMoxingX_ = 0.0f;

  ComponentPtr buildButton(std::string text,
                           std::function<void(const UITapEvent&)> onTap,
                           Color color = Color::Green()) {
    return IgniteUI::View({.backgroundColor = color, .borderRadius = 12, .onTap = onTap})({
        IgniteUI::Box({.size = UISizing(120, 40), .alignment = UIAlignment::Center})({
            IgniteUI::Paragraphs::text(text),
        }),
    });
  }
};

class AnimationExample : public StatefullyComponent {
 public:
  AnimationExample() : StatefullyComponent(AUTO_KEY_NAMED("animation-example")) {}

  std::shared_ptr<StateBase> createState() override { return std::make_shared<AnimationExampleStateAdvanced>(); }
};
