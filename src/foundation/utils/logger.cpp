#include "foundation/utils/logger.hpp"

#include <fmt/core.h>

void Logger::log(const char *message) { fmt::println("{}\n", message); }
