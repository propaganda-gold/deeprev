#ifndef util_logging_h
#define util_logging_h
#include "absl/strings/str_cat.h"
#include "absl/types/optional.h"
#include <iostream>

namespace vectorbook {

template <typename T>
std::string OptionalString(const absl::optional<T> &t_opt) {
  if (t_opt.has_value()) {
    return absl::StrCat("[", *t_opt, "]");
  } else {
    return "[none]";
  }
}


// Print_ is for debugging only. Print_ lines should either not checked in at
// all or cleared arbitrarily often.
#define Print_line(expr)                                                       \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << std::endl;

#define Print_value(expr)                                                      \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << (expr) << std::endl;

#define Print_status(expr)                                                      \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << (expr).status() << std::endl;

#define Print_opt(expr)                                                        \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << OptionalString(expr) << std::endl;

#define Print_coln(expr)                                                       \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] {" << Join(expr, ", ") \
            << "}" << std::endl;

#define Print_debug(expr)                                                      \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr             \
            << ".DebugString(): " << (expr).DebugString() << std::endl;

#define Print_size(expr)                                                       \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << (expr).size() << std::endl;

#define Print_msg(msg)                                                         \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << (msg) << std::endl;

#define Print_each(expr)                                                       \
  for (const auto &element : (expr)) {                                         \
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "   \
              << element << std::endl;                                         \
  }

#define Output_line(expr)                                                       \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << std::endl;

#define Output_value(expr)                                                      \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << (expr) << std::endl;

#define Output_status(expr)                                                      \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << (expr).status() << std::endl;

#define Output_opt(expr)                                                        \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << OptionalString(expr) << std::endl;

#define Output_coln(expr)                                                       \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] {" << Join(expr, ", ") \
            << "}" << std::endl;

#define Output_debug(expr)                                                      \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr             \
            << ".DebugString(): " << (expr).DebugString() << std::endl;

#define Output_size(expr)                                                       \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "     \
            << (expr).size() << std::endl;

#define Output_msg(msg)                                                         \
  std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << (msg) << std::endl;

#define Output_each(expr)                                                       \
  for (const auto &element : (expr)) {                                         \
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << #expr << ": "   \
              << element << std::endl;                                         \
  }

// Output to the CLI user.
#define Output_() std::cout << "[" << __FILE__ << ":" << __LINE__ << "] "

// Error message. This is a big deal. An error message in logs means a bug
// should be filed and fixed as P0.
#define Error_() std::cout << "[" << __FILE__ << ":" << __LINE__ << "] "

// Warning message. Characterize a case of concern, which is not necessarily a
// bug.
#define Warning_() std::cout << "[" << __FILE__ << ":" << __LINE__ << "] "

// Messages that run in prod, but do not indicate a bug or a concern.
#define Info_() std::cout << "[" << __FILE__ << ":" << __LINE__ << "] "

std::ostream &GenericLog(const std::string &stream,
                         const std::string &file_name, int line);

#define Info() GenericLog("INFO", __FILE__, __LINE__)

} // namespace vectorbook
#endif /* util_logging_h */
