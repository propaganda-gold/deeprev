#include "util/logging.h"

namespace vectorbook {

std::ostream &GenericLog(const std::string &stream,
                         const std::string &file_name, int line) {
  return std::cout << stream << " " << file_name << " " << line << ": ";
}

} // namespace vectorbook
