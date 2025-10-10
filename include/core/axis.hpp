#pragma once

#include <cstdint>
#include <ostream>

enum class Axis : std::uint8_t { HORIZONTAL, VERTICAL };

inline std::ostream &operator<<(std::ostream &os, const Axis &axis) {
  switch (axis) {
    case Axis::HORIZONTAL:
      os << "HORIZONTAL";
      break;
    case Axis::VERTICAL:
      os << "VERTICAL";
      break;
  }

  return os;
}

inline std::string axisToString(Axis axis) {
  switch (axis) {
    case Axis::HORIZONTAL:
      return "HORIZONTAL";
    case Axis::VERTICAL:
      return "VERTICAL";
  }
}
