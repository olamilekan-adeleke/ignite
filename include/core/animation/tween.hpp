#include <algorithm>
#include <type_traits>
#include <utility>

namespace IgniteAnimation {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type lerp(const T& a, const T& b, double t) {
  return a + (b - a) * static_cast<T>(t);
}

template <typename T>
class Tween {
 public:
  Tween() : start_(T()), end_(T()), progress_(0.0) {}
  Tween(T start, T end) : start_(std::move(start)), end_(std::move(end)), progress_(0.0) {}

  void setValues(T start, T end) {
    start_ = std::move(start);
    end_ = std::move(end);
  }

  void setProgress(double progress) { progress_ = std::clamp(progress, 0.0, 1.0); }

  T getValue() const { return lerp(start_, end_, progress_); }

  double getProgress() const { return progress_; }

 private:
  T start_;
  T end_;
  double progress_;
};
}  // namespace IgniteAnimation
