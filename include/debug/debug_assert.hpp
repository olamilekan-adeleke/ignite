#pragma once
#include <cxxabi.h>
#include <execinfo.h>
#include <fmt/base.h>
#include <unistd.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "component/component.hpp"
#include "render/render_object.hpp"

#ifdef __APPLE__
#include <libproc.h>
#include <mach-o/dyld.h>
#endif

inline void printRenderTree(const RenderObjectPtr& ro, int indent = 0) {
  if (!ro) return;

  std::string pad(indent * 2, ' ');
  fmt::println("{}├─ {}", pad, typeid(*ro).name());

  // Assuming RenderObject has a way to get children
  // Adjust based on your actual RenderObject API
  for (int i = 0; i < ro->getChildren().size(); ++i) {
    if (auto child = ro->getChild(i)) {
      printRenderTree(child, indent + 1);
    }
  }
}

// Print the element tree
inline void printElementTree(const UIElementPtr& elem, int indent = 0) {
  if (!elem) return;

  std::string pad(indent * 2, ' ');
  auto comp = elem->getComponont();
  auto ro = elem->getRenderObject();

  fmt::println("{}├─ Element: {} | Component: {} | RenderObject: {}",
               pad,
               typeid(*elem).name(),
               comp ? typeid(*comp).name() : "null",
               ro ? typeid(*ro).name() : "null");

  // Print children (you may need to expose children vector or add a getter)
}

inline void printStackTraceTT() {
  void* callstack[128];
  int frames = backtrace(callstack, 128);
  char** strs = backtrace_symbols(callstack, frames);

  fmt::println("Stack trace:");
  for (int i = 0; i < frames; ++i) {
    // Demangle the symbol
    char* demangled = nullptr;
    for (char* p = strs[i]; *p; ++p) {
      if (*p == '_' && *(p + 1) == 'Z') {
        // Found mangled name, extract it
        char* end = p;
        while (*end && *end != ' ' && *end != '+') end++;

        size_t len = end - p;
        char* mangled = static_cast<char*>(malloc(len + 1));
        strncpy(mangled, p, len);
        mangled[len] = '\0';

        int status;
        demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);

        if (status == 0 && demangled) {
          fmt::println("  {}: {}", i, demangled);
          free(demangled);
        } else {
          fmt::println("  {}: {}", i, strs[i]);
        }
        free(mangled);
        break;
      }
    }
    if (!demangled) {
      fmt::println("  {}: {}", i, strs[i]);
    }
  }
  free(strs);
}

// Helper to demangle C++ names
inline std::string demangle(const char* name) {
  int status = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
  std::string result = (status == 0 && demangled) ? demangled : name;
  free(demangled);
  return result;
}

// Fixed executable path retrieval for macOS
inline std::string get_executable_path() {
#ifdef __APPLE__
  // Method 1: Use _NSGetExecutablePath (macOS specific)
  uint32_t size = 0;
  _NSGetExecutablePath(nullptr, &size);
  if (size > 0) {
    std::vector<char> path(size);
    if (_NSGetExecutablePath(path.data(), &size) == 0) {
      return std::string(path.data());
    }
  }

  // Method 2: Use proc_pidpath (macOS specific backup)
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidpath(getpid(), pathbuf, sizeof(pathbuf)) > 0) {
    return std::string(pathbuf);
  }
#endif

  // Method 3: Linux approach (won't work on macOS but kept for completeness)
  std::vector<char> path(512);
  ssize_t len = readlink("/proc/self/exe", path.data(), path.size());
  if (len != -1) {
    return std::string(path.data(), len);
  }

  return "";
}

inline void print_stacktrace_atos() {
  void* array[50];
  size_t size = backtrace(array, 50);

  std::string executable_path = get_executable_path();
  if (executable_path.empty()) {
    std::cerr << "Could not get executable path, falling back to basic trace\n";

    // Fallback: basic symbol trace
    char** symbols = backtrace_symbols(array, size);
    if (symbols) {
      for (size_t i = 0; i < size; ++i) {
        std::cerr << "#" << i << " " << symbols[i] << std::endl;
      }
      free(symbols);
    }
    return;
  }

  std::cerr << "Stack trace:\n";

  // Try each address individually with atos for better results
  for (size_t i = 0; i < size; ++i) {
    // Build atos command with flags for detailed output
    std::string command = "atos -o '" + executable_path + "' -l 0x0 -fullPath " + std::to_string((uintptr_t)array[i]);

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (pipe) {
      std::array<char, 1024> buffer;
      if (fgets(buffer.data(), buffer.size(), pipe.get())) {
        std::string line(buffer.data());
        line.erase(line.find_last_not_of(" \n\r\t") + 1);

        // If atos failed to resolve, show the raw address with backtrace_symbols info
        if (line.find("0x") == 0 || line.find("???") != std::string::npos) {
          char** symbols = backtrace_symbols(&array[i], 1);
          if (symbols) {
            std::cerr << "#" << i << " " << demangle(symbols[0]) << " [" << std::hex << (uintptr_t)array[i] << std::dec
                      << "]" << std::endl;
            free(symbols);
          } else {
            std::cerr << "#" << i << " " << line << std::endl;
          }
        } else {
          std::cerr << "#" << i << " " << line << std::endl;
        }
      }
    }
  }

  // Alternative: try llvm-addr2line if available
  std::cerr << "\n--- Alternative with addr2line ---\n";
  std::string addr2line_cmd = "which addr2line > /dev/null 2>&1";
  if (system(addr2line_cmd.c_str()) == 0) {
    for (size_t i = 0; i < size; ++i) {
      std::string command = "addr2line -f -C -e '" + executable_path + "' " + std::to_string((uintptr_t)array[i]);

      std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
      if (pipe) {
        std::array<char, 1024> buffer;
        std::string function_name, file_line;

        if (fgets(buffer.data(), buffer.size(), pipe.get())) {
          function_name = buffer.data();
          function_name.erase(function_name.find_last_not_of(" \n\r\t") + 1);
        }
        if (fgets(buffer.data(), buffer.size(), pipe.get())) {
          file_line = buffer.data();
          file_line.erase(file_line.find_last_not_of(" \n\r\t") + 1);
        }

        if (!function_name.empty() && function_name != "??") {
          std::cerr << "#" << i << " " << function_name << " at " << file_line << std::endl;
        }
      }
    }
  }
}

#define VERIFY(condition, message)                                                             \
  do {                                                                                         \
    if (condition) {                                                                           \
      std::cerr << "WARNING UI: " << message << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    }                                                                                          \
  } while (0)
