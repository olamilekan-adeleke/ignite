#pragma once

#include <string>

class UIKey {
 public:
  UIKey();
  explicit UIKey(const std::string value) : value_(value) {};

  std::string value() const;

  bool operator==(const UIKey &other) const;
  bool operator!=(const UIKey &other) const;

  [[nodiscard]] std::string toString() const noexcept;

 private:
  std::string value_;

  std::string gererateRandomKey();
};
