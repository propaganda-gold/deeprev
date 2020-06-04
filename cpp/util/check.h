#ifndef util_check_h
#define util_check_h
#include "coln/api.h"
#include "util/includes.h"

namespace vectorbook {

unique_ptr<Error> CheckImpl(const string &file, int line,
                            const Checkable &obj) {
  if (obj.has_error()) {
    auto result = absl::make_unique<Error>(obj.error());
		result->add_location(file, line);
		return result;
  } else {
    return nullptr;
  }
}

#define Check(expr)                                                            \
  {                                                                            \
    auto check_impl = CheckImpl(__FILE__, __LINE__, (expr));                   \
    if (check_impl != nullptr) {                                               \
      return *check_impl;                                                      \
    }                                                                          \
  }

} // namespace vectorbook
#endif /* util_check_h */
