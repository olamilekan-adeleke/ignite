#pragma once
#include <fstream>

class Logger {
 public:
  static void log(const char* message);

  static void logToFile(const std::string& message, const std::string& filename = "debug.log") {
    std::ofstream file(filename, std::ios::trunc);
    file << message << std::endl;
    file.close();
  }
};
