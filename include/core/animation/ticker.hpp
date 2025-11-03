#pragma once

#include <algorithm>
#include <vector>

#include "animation/tick_listener.hpp"

namespace IgniteAnimation {

class Ticker {
 public:
  void addListener(TickListener* listener) {
    if (!listener) return;
    auto it = std::find(listeners_.begin(), listeners_.end(), listener);
    if (it == listeners_.end()) {
      listeners_.push_back(listener);
    }
  }

  void removeListener(TickListener* listener) {
    auto it = std::find(listeners_.begin(), listeners_.end(), listener);
    if (it != listeners_.end()) listeners_.erase(it);
  }

  void tick(double delta) {
    for (auto listener : listeners_) listener->onTick(delta);
  }

 private:
  std::vector<TickListener*> listeners_;
};

}  // namespace IgniteAnimation
