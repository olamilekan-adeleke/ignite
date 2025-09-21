#pragma once

#include <cstdint>
#include <string>
#include <iomanip>
#include <sstream>

inline uint64_t fnv1a(const std::string& data) {
  // uint64_t hash = 1469598103934665603ull;
  // for (unsigned char c : data) {
  //   hash ^= c;
  //   hash *= 1099511628211ull;
  // }
  // return hash;
  const auto val = std::hash<std::string>{}(data);
  return val;
}

inline std::string toHex(uint64_t hash) {
  std::ostringstream oss;
  oss << std::hex << std::setw(16) << std::setfill('0') << hash;
  return oss.str();
}

class HasHashCodes {
 public:
  virtual ~HasHashCodes() = default;

  virtual uint64_t layoutHashCode() const noexcept = 0;
  virtual uint64_t drawHashCode() const noexcept = 0;
};
