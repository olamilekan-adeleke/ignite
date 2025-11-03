#include "core/animation/animation_controller.hpp"

#include "core/animation/curves.hpp"

namespace IgniteAnimation {
AnimationController::AnimationController()
    : state_(AnimationState::Idle),
      direction_(AnimationDirection::Forward),
      duration_(1.0),
      elapsedTime_(0.0),
      progress_(0.0),
      curve_(Curves::linear),
      hasStarted_(false) {}

void AnimationController::setDuration(double durationSeconds) {
  duration_ = durationSeconds > 0 ? durationSeconds : 1.0;
}

void AnimationController::setCurve(std::function<double(double)> curve) {
  if (curve) curve_ = curve;
}

void AnimationController::setDirection(AnimationDirection direction) { direction_ = direction; }

void AnimationController::play() {
  if (state_ != AnimationState::Running) {
    if (state_ == AnimationState::Completed) {
      elapsedTime_ = 0.0;
      progress_ = 0.0;
      hasStarted_ = false;
      direction_ = AnimationDirection::Forward;
    }
    state_ = AnimationState::Running;
    if (!hasStarted_ && onStart_) {
      onStart_();
      hasStarted_ = true;
    }
  }
}

void AnimationController::pause() {
  if (state_ == AnimationState::Running) state_ = AnimationState::Paused;
}

void AnimationController::stop() {
  state_ = AnimationState::Idle;
  elapsedTime_ = 0.0;
  progress_ = 0.0;
  hasStarted_ = false;
  if (onUpdate_) onUpdate_(progress_);
}

void AnimationController::reverse() {
  if (state_ != AnimationState::Completed) return;

  direction_ = (direction_ == AnimationDirection::Forward) ? AnimationDirection::Reverse : AnimationDirection::Forward;
  elapsedTime_ = (direction_ == AnimationDirection::Forward) ? 0.0 : duration_;
  state_ = AnimationState::Running;
}

void AnimationController::setOnStart(std::function<void()> callback) { onStart_ = callback; }
void AnimationController::setOnUpdate(std::function<void(double)> callback) { onUpdate_ = callback; }
void AnimationController::setOnComplete(std::function<void()> callback) { onComplete_ = callback; }
double AnimationController::getEasedProgress() const { return curve_(progress_); }

void AnimationController::onTick(double deltaTime) {
  if (state_ != AnimationState::Running) return;
  elapsedTime_ += (direction_ == AnimationDirection::Forward ? deltaTime : -deltaTime);

  if (elapsedTime_ < 0) {
    elapsedTime_ = 0;
    progress_ = 0.0;
    state_ = AnimationState::Completed;
    if (onComplete_) onComplete_();
    return;
  } else if (elapsedTime_ >= duration_) {
    elapsedTime_ = duration_;
    progress_ = 1.0;
    state_ = AnimationState::Completed;
    if (onComplete_) onComplete_();
    return;
  }

  progress_ = elapsedTime_ / duration_;
  if (onUpdate_) onUpdate_(getEasedProgress());
}
}  // namespace IgniteAnimation
