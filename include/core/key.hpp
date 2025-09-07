#pragma once

#include <string>

class UIKey {
 public:
  UIKey();
  explicit UIKey(const std::string value) : value_(value) {};

  std::string value() const;

  bool operator==(const UIKey &other) const;
  bool operator!=(const UIKey &other) const;

  std::string toString() const;

 private:
  std::string value_;

  std::string gererateRandomKey();
};
