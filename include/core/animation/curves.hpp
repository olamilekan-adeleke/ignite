#pragma once

namespace IgniteAnimation {

namespace Curves {

inline double linear(double t) { return t; }
inline double easeInQuad(double t) { return t * t; }
inline double easeOutQuad(double t) { return 1.0 - (1.0 - t) * (1.0 - t); }

// TODO: add more later

}  // namespace Curves
}  // namespace IgniteAnimation
