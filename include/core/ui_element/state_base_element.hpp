#pragma once

#include "component/component.hpp"

class StatelessUIElement : public UIElement {
 public:
  explicit StatelessUIElement(ComponentPtr c);

  void mount(UIElementPtr element) noexcept override;
  void unmount() noexcept override;
  void update(ComponentPtr newComp) noexcept override;
  void performRebuild() noexcept override;

  void debugFillProperties(std::ostringstream& os, int indent) const noexcept {
    UIElement::debugFillProperties(os, indent);
  }
};

class StatefulUIElement : public UIElement {
 public:
  explicit StatefulUIElement(ComponentPtr c);

  void mount(UIElementPtr element) noexcept override;
  void unmount() noexcept override;
  void update(ComponentPtr newComp) noexcept override;
  void performRebuild() noexcept override;
};

class LeafUIElement : public UIElement {
 public:
  explicit LeafUIElement(ComponentPtr c);

  void performRebuild() noexcept override;
};
