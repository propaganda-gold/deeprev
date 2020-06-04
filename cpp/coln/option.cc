#include "coln/option.h"
#include <iostream>

namespace vectorbook {

Error Ok() { return Error(); }

std::unique_ptr<Error> CheckImpl(const std::string &file, int line,
                                 Checkable &checkable) {
  if (checkable.has_error()) {
    auto result = absl::make_unique<Error>(checkable.error());
    result->add_location(file, line);
    return result;
  } else {
    return nullptr;
  }
}

std::unique_ptr<Error> CheckExistsImpl(const std::string &expr_name,
                                       const std::string &file, int line,
                                       Checkable &checkable) {
  if (checkable.has_error()) {
    auto result = absl::make_unique<Error>(checkable.error());
    result->add_location(file, line);
    return result;
  }
  if (!checkable.has_value()) {
		std::string error_msg = expr_name + " is empty.";
    auto result = absl::make_unique<Error>(file, line, error_msg);
    // result->add_location(file, line);
    return result;
  }
  return nullptr;
}

std::string PrettyStringFromError(Checkable &checkable) {
  std::string buffer;
  if (checkable.has_error()) {
    const Error &error = checkable.error();
    buffer += error.message();
    buffer += "\n";
    buffer += error.StackTrace();
    buffer += "\n";
  } else {
    buffer = "ASSERT_OK";
  }
  return buffer;
}

void PrettyPrintError(Checkable &checkable) {
  std::cout << PrettyStringFromError(checkable) << "\n";
}

} // namespace vectorbook
