#include "core/ui_element/state_base_element.hpp"

#include "component/state_base_component.hpp"

// -- StatelessUIElement Implementation --
StatelessUIElement::StatelessUIElement(ComponentPtr c) : UIElement(c) {}

void StatelessUIElement::mount(UIElementPtr element) noexcept {
  UIElement::mount(element);
  performRebuild();
}

void StatelessUIElement::unmount() noexcept {
  UIElement::unmount();
  // performRebuild();
}

void StatelessUIElement::performRebuild() noexcept {
  auto sc = std::dynamic_pointer_cast<StatelessComponent>(getComponont());
  if (sc) {
    auto children = {sc->build()};
    this->setElementChildren(updateChildren(children));
  }
}

void StatelessUIElement::update(ComponentPtr newComp) noexcept {
  UIElement::update(newComp);
  performRebuild();
}

// -- StatefulUIElement Implementation --
StatefulUIElement::StatefulUIElement(ComponentPtr c) : UIElement(c) {}

void StatefulUIElement::mount(UIElementPtr element) noexcept {
  UIElement::mount(element);
  auto sc = std::dynamic_pointer_cast<StatefullyComponent>(getComponont());
  if (sc) {
    state = sc->createState();
    if (state) {
      state->element_ = this;
      state->initState();
    }
  }
  performRebuild();
}

void StatefulUIElement::unmount() noexcept {
  if (state) state->dispose();
  UIElement::unmount();
}

void StatefulUIElement::update(ComponentPtr newComp) noexcept {
  UIElement::update(newComp);
  performRebuild();
}

void StatefulUIElement::performRebuild() noexcept {
  if (state) {
    const auto& children = {state->build()};
    this->setElementChildren(updateChildren(children));
  }
}

// -- LeafUIElement Implementation --
LeafUIElement::LeafUIElement(ComponentPtr c) : UIElement(c) {}

void LeafUIElement::performRebuild() noexcept { setElementChildren({}); }
