#include "component/state_base_component.hpp"

#include <fmt/base.h>

StateBase::~StateBase() = default;

void StateBase::updateState(const std::function<void()>& fn) {
  if (fn) fn();
  if (element_) element_->performRebuild();
}

UIElementPtr StatelessComponent::createElement() { return std::make_shared<StatelessUIElement>(shared_from_this()); };

UIElementPtr StatefullyComponent::createElement() { return std::make_shared<StatefulUIElement>(shared_from_this()); };
