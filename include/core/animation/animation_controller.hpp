#pragma once

#include <functional>

#include "animation/tick_listener.hpp"

namespace IgniteAnimation {

enum class AnimationState { Idle, Running, Paused, Completed };

enum class AnimationDirection { Forward, Reverse };

class AnimationController : public TickListener {
 public:
  AnimationController();

  void setDuration(double durationSeconds);
  void setCurve(std::function<double(double)> curve);
  void setDirection(AnimationDirection direction);

  void play();
  void pause();
  void stop();
  void reverse();

  AnimationState getState() const { return state_; }
  double getProgress() const { return progress_; }
  double getElapsedTime() const { return elapsedTime_; }
  double getDuration() const { return duration_; }

  void setOnStart(std::function<void()> callback);
  void setOnUpdate(std::function<void(double)> callback);
  void setOnComplete(std::function<void()> callback);

  void onTick(double deltaTime) override;

  double getEasedProgress() const;

 private:
  AnimationState state_;
  AnimationDirection direction_;
  double duration_;
  double elapsedTime_;
  double progress_;
  std::function<double(double)> curve_;

  std::function<void()> onStart_;
  std::function<void(double)> onUpdate_;
  std::function<void()> onComplete_;

  bool hasStarted_;
};

}  // namespace IgniteAnimation
