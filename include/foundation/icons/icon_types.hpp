#pragma once

#include "icon_data.hpp"

struct IconTypes {
  static IconData home() { return {.unicode = 0xe88a}; }
  static IconData search() { return {.unicode = 0xe8b6}; }
  static IconData settings() { return {.unicode = 0xe8b8}; }
  static IconData favorite() { return {.unicode = 0xe87d}; }
  static IconData person() { return {.unicode = 0xe7fd}; }
  static IconData notifications() { return {.unicode = 0xe7f4}; }
  static IconData menu() { return {.unicode = 0xe5d2}; }
  static IconData close() { return {.unicode = 0xe5cd}; }
  static IconData check() { return {.unicode = 0xe5ca}; }
  static IconData add() { return {.unicode = 0xe145}; }
  static IconData edit() { return {.unicode = 0xe3c9}; }
};
