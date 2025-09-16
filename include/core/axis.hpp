#include <cstdint>
#include <ostream>

enum class Axis : std::uint8_t { HORIZONTAL, VERTICAL };

inline std::ostream& operator<<(std::ostream& os, const Axis& axis) {
  switch (axis) {
    case Axis::HORIZONTAL:
      os << "Axis::HORIZONTAL";
      break;
    case Axis::VERTICAL:
      os << "Axis::VERTICAL";
      break;
  }

  return os;
}

inline std::string axisToString(Axis axis) {
  switch (axis) {
    case Axis::HORIZONTAL:
      return "Axis::HORIZONTAL";
    case Axis::VERTICAL:
      return "Axis::VERTICAL";
  }
}
