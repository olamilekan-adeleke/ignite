#pragma once

#include <cstddef>
#include <string>

// Helper function to count bytes in a UTF-8 character
inline size_t utf8CharLength(const char* str) noexcept {
  unsigned char c = static_cast<unsigned char>(str[0]);
  if (c < 0x80) return 1;
  if ((c & 0xE0) == 0xC0) return 2;
  if ((c & 0xF0) == 0xE0) return 3;
  if ((c & 0xF8) == 0xF0) return 4;
  return 1;
}

inline std::string extractUtf8Char(const std::string& input, size_t& pos) noexcept {
  if (pos >= input.size()) return "";
  size_t charLen = utf8CharLength(&input[pos]);
  std::string result = input.substr(pos, charLen);
  pos += charLen;
  return result;
}
