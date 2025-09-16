#include <iostream>

// clang-format off
#define VERIFY(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "WARNING UI: " << #condition \
                       << " " <<message << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while (0)
// clang-format on
