#include <memory>
#include <vector>

#include "interactive_components/stateful_component.hpp"
#include "ui.hpp"
#include "ui_alignment.hpp"
#include "ui_edge_insets.hpp"

struct NavigationItem {
  std::shared_ptr<UIComponent> sidebarItem;
  UIEdgeInsets sidebarItemPadding = UIEdgeInsets::vertical(6) + UIEdgeInsets::horizontal(8);
  std::shared_ptr<UIComponent> view;
  std::optional<VoidCallBack> onTap;
};

struct NavigationViewParam {
  std::vector<NavigationItem> navigationItems;
  UIEdgeInsets sidebarPadding = UIEdgeInsets::vertical(10) + UIEdgeInsets::horizontal(16);
  float sidebarSpacing = 10;
  UIEdgeInsets contentPadding = UIEdgeInsets::vertical(20) + UIEdgeInsets::horizontal(16);
};

class NavigationView : public StatefulComponent {
 public:
  NavigationView(const NavigationViewParam& params) : params_(params) {}

  std::shared_ptr<UIComponent> body() override {
    return UI::HFlexBox({
        .children =
            {
                UI::FixedBoxView({
                    .child = UI::UIView({.insets = params_.sidebarPadding, .child = buildSidebar()}),
                    .alignment = UIAlignment::TopLeft,
                }),

                UI::UISeparator({}),

                buildView(),
            },

    });
  }

 private:
  NavigationViewParam params_;
  int selectedIndex_ = 0;

  std::shared_ptr<UIComponent> buildSidebar() noexcept {
    const auto& navItems = params_.navigationItems;
    std::vector<std::shared_ptr<UIComponent>> sidebarItems;

    for (int i = 0; i < params_.navigationItems.size(); ++i) {
      auto onTap = [this, i, navItems](const UITapEvent&) {
        if (navItems[i].onTap) {
          navItems[i].onTap.value()();
        }
        selectedIndex_ = i;
        markDirty();
      };

      const auto child = UI::UIView({
          .insets = navItems[i].sidebarItemPadding + UIEdgeInsets::horizontal(20),
          .backgroundColor = Color::LightGray(),
          .borderRadius = 10,
          .onTap = onTap,
          .child = navItems[i].sidebarItem,
      });

      sidebarItems.push_back(child);
    }
    return UI::VFlexBox({
        .spacing = params_.sidebarSpacing,
        .crossAxisAlignment = CrossAxisAlignment::START,
        .children = sidebarItems,
    });
  }

  std::shared_ptr<UIComponent> buildView() noexcept {
    const auto& navItems = params_.navigationItems;
    if (navItems.empty() || selectedIndex_ > navItems.size()) {
      return UI::UIView({});
    }

    return UI::UIView({
        .insets = params_.contentPadding,
        .child = navItems[selectedIndex_].view,
    });
  }
};
