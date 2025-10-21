#pragma once

#include <string>

class KeyPool {
 public:
  static KeyPool& instance() {
    static KeyPool pool;
    return pool;
  }

  void reset() { counter = 0; }

  std::string next() { return "key_" + std::to_string(counter++); }

  std::string next(const std::string& prefix) { return prefix + "_" + std::to_string(counter++); }

 private:
  KeyPool() = default;
  int counter = 0;
};

#define AUTO_KEY UIKey(KeyPool::instance().next())
#define AUTO_KEY_NAMED(name) UIKey(KeyPool::instance().next(name))

class UIKey {
 public:
  UIKey();
  explicit UIKey(const std::string value) : value_(value) {};

  std::string value() const;

  bool operator==(const UIKey& other) const;
  bool operator!=(const UIKey& other) const;

  operator std::string() const { return value_; }

  [[nodiscard]] std::string toString() const noexcept;

 private:
  std::string value_;

  std::string gererateRandomKey();
};
