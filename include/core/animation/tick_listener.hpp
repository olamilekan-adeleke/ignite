#pragma once

namespace IgniteAnimation {

class TickListener {
 public:
  virtual void onTick(double delta) = 0;

  ~TickListener() = default;
};

}  // namespace IgniteAnimation
