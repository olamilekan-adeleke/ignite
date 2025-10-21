#pragma once
#include <algorithm>
#include <fstream>
#include <mutex>
#include <vector>

class Logger {
 public:
  static void log(const char* message);

  static void logToFile(const std::string& message, const std::string& filename = "debug.log") {
    std::ofstream file(filename, std::ios::trunc);
    file << message << std::endl;
    file.close();
  }
};

enum class LogCategory { General, Timer };

struct LogEntry {
  LogCategory category;
  std::string message;
  std::chrono::steady_clock::time_point timestamp;
  std::chrono::milliseconds lifespan;  // 0 = persistent

  LogEntry(LogCategory cat, std::string msg, std::chrono::milliseconds life = std::chrono::milliseconds{0})
      : category(cat), message(std::move(msg)), timestamp(std::chrono::steady_clock::now()), lifespan(life) {}
};

class UILogger {
 public:
  static UILogger& instance() {
    static UILogger logger;
    return logger;
  }

  // --- Public API ---
  void time(const std::string& msg, std::chrono::milliseconds life = std::chrono::milliseconds{1000}) {
    upsert(LogCategory::Timer, msg, life);
  }

  void info(const std::string& msg, std::chrono::milliseconds life = std::chrono::milliseconds{1000}) {
    upsert(LogCategory::General, msg, life);
  }

  void permanent(const std::string& msg, LogCategory category = LogCategory::General) {
    upsert(category, msg, std::chrono::milliseconds{0});
  }

  // --- Maintenance ---
  void update() {
    std::scoped_lock lock(mutex_);
    auto now = std::chrono::steady_clock::now();
    transientLogs_.erase(
        std::remove_if(transientLogs_.begin(),
                       transientLogs_.end(),
                       [&](const LogEntry& e) { return e.lifespan.count() > 0 && now - e.timestamp > e.lifespan; }),
        transientLogs_.end());
  }

  std::vector<LogEntry> getTransientLogs() const {
    std::scoped_lock lock(mutex_);
    return transientLogs_;
  }

  std::vector<LogEntry> getPersistentLogs() const {
    std::scoped_lock lock(mutex_);
    return persistentLogs_;
  }

 private:
  mutable std::mutex mutex_;
  std::vector<LogEntry> persistentLogs_;
  std::vector<LogEntry> transientLogs_;

  void upsert(LogCategory category, const std::string& msg, std::chrono::milliseconds life) {
    std::scoped_lock lock(mutex_);
    auto& container = (life.count() == 0) ? persistentLogs_ : transientLogs_;

    auto it = std::find_if(container.begin(), container.end(), [&](const LogEntry& e) {
      return e.category == category && e.message == msg;
    });

    if (it != container.end()) {
      it->timestamp = std::chrono::steady_clock::now();
      it->message = msg;
    } else {
      container.emplace_back(category, msg, life);
    }
  }
};
