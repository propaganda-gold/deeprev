#ifndef util_stacktrace_h
#define util_stacktrace_h
#include <iostream>
#include <string>

namespace vectorbook {

std::string StackTrace();

// An "raw" c++ assert is one not following our "Checkable" framework of
// recovering a stack  trace using macros.
//
// A "raw" assert will not typically return a useful stacktrace.
//
// In production, the type "string" should be caught somewhere, so that an
// assertion does not cause a crash.
//
// In production, a raw assert is preferable to a SEGFAULT, because a raw
// assert can be caught.
//
// A "raw" assert is appropriate only when failing inside of Checkable
// framework itself.
#define RAW_ASSERT(expr)                                                       \
  {                                                                            \
    if (!(expr)) {                                                             \
      std::cout << __FILE__ << __LINE__ << "\n";                               \
      std::cout << StackTrace() << "\n";                                       \
      throw(#expr);                                                            \
    }                                                                          \
  }

} // namespace vectorbook
#endif /* util_stacktrace_h */
