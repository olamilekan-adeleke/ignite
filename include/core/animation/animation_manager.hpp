#pragma once
#include <vector>

#include "animation/ticker.hpp"
#include "animation_controller.hpp"

namespace IgniteAnimation {

class AnimationManager {
 public:
  static AnimationManager& instance() {
    static AnimationManager manager;
    return manager;
  }

  void tick(double deltaTime) {
    for (auto it = controllers_.begin(); it != controllers_.end();) {
      auto controller = *it;
      controller->onTick(deltaTime);

      if (controller->getState() == AnimationState::Completed) {
        it = controllers_.erase(it);
      } else {
        ++it;
      }
    }
  }

  void registerController(AnimationController* controller) {
    if (controller && std::find(controllers_.begin(), controllers_.end(), controller) == controllers_.end()) {
      controllers_.push_back(controller);
    }
  }

  void unregisterController(AnimationController* controller) {
    auto it = std::find(controllers_.begin(), controllers_.end(), controller);
    if (it != controllers_.end()) controllers_.erase(it);
  }

  void clear() { controllers_.clear(); }

  int getActiveAnimationCount() const { return static_cast<int>(controllers_.size()); }

 private:
  AnimationManager() = default;
  Ticker ticker_;
  std::vector<AnimationController*> controllers_;
};

}  // namespace IgniteAnimation
