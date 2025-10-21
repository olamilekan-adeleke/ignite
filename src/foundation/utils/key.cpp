#include "foundation/utils/key.hpp"

#include <random>
#include <sstream>

UIKey::UIKey() : value_(std::move(gererateRandomKey())) {}

std::string UIKey::value() const { return value_; }

bool UIKey::operator==(const UIKey& other) const { return value_ == other.value_; }
bool UIKey::operator!=(const UIKey& other) const { return !(*this == other); }

std::string UIKey::gererateRandomKey() {
  std::stringstream ss;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 15);
  for (int i = 0; i < 16; ++i) ss << std::hex << dis(gen);
  return ss.str();
}

std::string UIKey::toString() const noexcept { return value_; }
