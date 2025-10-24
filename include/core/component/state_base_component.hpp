#pragma once

#include <memory>

#include "component/component.hpp"
#include "ui_element/state_base_element.hpp"

class StateBase {
 public:
  StatefulUIElement* element_ = nullptr;

  virtual ~StateBase();

  virtual void initState() {}

  virtual void dispose() {}

  virtual ComponentPtr build() = 0;

  void updateState(const std::function<void()>& fn);
};

// STATE LESS COMPONENT
class StatelessComponent : public Component {
 public:
  StatelessComponent(const UIKey& key = AUTO_KEY) : Component(key) {}

  UIElementPtr createElement() override;

  RenderObjectPtr createRenderObject() const noexcept override = 0;

  virtual ComponentPtr build() = 0;
};

// STATE FULLY COMPONENT
class StatefullyComponent : public Component {
 public:
  StatefullyComponent(const UIKey& key = AUTO_KEY) : Component(key) {}

  UIElementPtr createElement() override;

  virtual std::shared_ptr<StateBase> createState() = 0;
};
