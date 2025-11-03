#pragma once

#include <functional>

#include "animation_manager.hpp"
#include "tween.hpp"

namespace IgniteAnimation {
template <typename T>
class Animation {
 public:
  Animation() : controller_(), tween_() {}
  ~Animation() { AnimationManager::instance().unregisterController(&controller_); }

  Animation(const T& start, const T& end, double durationSeconds) : controller_(), tween_(start, end) {
    controller_.setDuration(durationSeconds);
    controller_.setOnUpdate([this](double progress) { tween_.setProgress(progress); });
  }

  void setValues(const T& start, const T& end) { tween_.setValues(start, end); }
  void setDuration(double duration) { controller_.setDuration(duration); }
  void setCurve(std::function<double(double)> curve) { controller_.setCurve(curve); }

  void play() {
    controller_.stop();
    AnimationManager::instance().registerController(&controller_);
    controller_.play();
  }

  void stop() {
    controller_.stop();
    AnimationManager::instance().unregisterController(&controller_);
  }

  void pause() { controller_.pause(); }

  void reverse() {
    if (controller_.getState() != AnimationState::Running) {
      AnimationManager::instance().registerController(&controller_);
    }
    controller_.reverse();
  }

  T getValue() const { return tween_.getValue(); }
  double getProgress() const { return controller_.getProgress(); }
  AnimationState getState() const { return controller_.getState(); }

  void setOnStart(std::function<void()> cb) { controller_.setOnStart(cb); }

  void setOnUpdate(std::function<void(double)> cb) {
    auto originalCallback = [this](double p) { tween_.setProgress(p); };
    controller_.setOnUpdate([this, cb, originalCallback](double p) {
      originalCallback(p);
      if (cb) cb(p);
    });
  }

  void setOnComplete(std::function<void()> callback) { controller_.setOnComplete(callback); }

  AnimationController& getController() { return controller_; }

 private:
  AnimationController controller_;
  Tween<T> tween_;
};
}  // namespace IgniteAnimation
